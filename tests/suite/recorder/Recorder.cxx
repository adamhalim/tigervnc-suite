#include "Recorder.h"
#include "util.h"
#include "tx/TXWindow.h"
#include "x0vncserver/Image.h"
#include <ios>
#include <chrono>
#include <thread>
#include <X11/Xlib.h>
#include <sys/select.h>

namespace suite {

  Recorder::Recorder(std::string filename, ImageDecoder* decoder,
                     std::string display, int framerate)
    : factory(true), decoder(decoder), interval(1000.0/framerate),
      intervalThreshold(interval/1000), lastImage(0)
  {
    // XOpenDisplay takes ownership of display string,
    // so we need to make a copy.
    char* displayCopy = new char[display.length() + 1];
    strncpy(displayCopy, display.c_str(), display.length());
    displayCopy[display.length()] = '\0';

    std::string dpyStr(displayCopy);
    if (!(dpy = XOpenDisplay(dpyStr.c_str() ? dpyStr.c_str() : 0))) {
      std::cerr << "unable to open display " << XDisplayName(dpyStr.c_str())
                << std::endl;
      exit(1);
    }

    geo = new Geometry(DisplayWidth(dpy, DefaultScreen(dpy)),
                       DisplayHeight(dpy, DefaultScreen(dpy)));
    fs = new FrameOutStream(filename, decoder);

    int xdamageErrorBase;
    if (!XDamageQueryExtension(dpy, &xdamageEventBase, &xdamageErrorBase)) {
      std::cerr << "DAMAGE extension not present" << std::endl;
      exit(1);
    }

    damage = XDamageCreate(dpy, DefaultRootWindow(dpy),
                           XDamageReportRawRectangles);
    TXWindow::init(dpy, "recorder");
  }

  Recorder::~Recorder()
  {
    delete fs;
    delete decoder;
  }

  void Recorder::startRecording()
  {
    try {
      fs->writeHeader(geo->width(), geo->height(), interval);
    } catch (std::logic_error &e) {
      // FIXME: Handle cases where startRecording() is called twice?
      throw;
    }

    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
    std::chrono::duration<double> sleepDuration = std::chrono::steady_clock
                                                     ::duration::zero();
    // FIXME: stopRecording() should stop the loop
    while (true) {
      start = std::chrono::steady_clock::now();
      std::vector<XEvent> events;
      while (XPending(dpy)) {
        XEvent event;
        XNextEvent(dpy, &event);
        events.push_back(event);
      }
      if (!events.size()) {
        continue;
      }

      handleEvents(events);
      end = std::chrono::steady_clock::now();
      auto encodeTime = std::chrono::duration<double, std::milli>(end - start);
      encodeTime_ = encodeTime.count();
      lastImageStats.encodingTime = encodeTime_;
      lastImageStats.margin = interval - encodeTime_;

#ifdef _DEBUG
        std::cout << "Encoding took: " << encodeTime.count() << std::endl;
#endif // _DEBUG
      // Sleep until next update
      if (encodeTime.count() + intervalThreshold <= interval) {
        struct timespec timespec;
        std::chrono::steady_clock::time_point sleepStart;
        std::chrono::steady_clock::time_point sleepEnd;

#ifdef _DEBUG
        double sleeptime = interval - intervalThreshold
                         - encodeTime.count() - sleepDuration.count();
          std::cout << "Sleep for: " << sleeptime << std::endl;
#endif // _DEBUG

        // 1e6: convert from milli to nanosec
        long sleep = ((interval - intervalThreshold) * 1e6)
                   - (encodeTime.count() * 1e6)
                   - (sleepDuration.count() * 1e6);

        timespec.tv_sec = 0;
        timespec.tv_nsec = sleep;

        sleepStart = std::chrono::steady_clock::now();
        pselect(0, 0, 0, 0, &timespec, 0);
        sleepEnd = std::chrono::steady_clock::now();
        sleepDuration = std::chrono::duration_cast
                       <std::chrono::milliseconds>(sleepEnd - sleepStart);
      } else {
        sleepDuration = std::chrono::steady_clock::duration::zero();
#ifdef _DEBUG
          std::cerr << "Frame not encoded within time interval\n";
#endif // _DEBUG
      }
    }
  }

  void Recorder::handleEvents(std::vector<XEvent>& events)
  {
    std::vector<rfb::Rect> rects;
    for (uint i = 0; i < events.size(); i++) {
      try {
        rects.push_back(rectFromEvent(events[i]));
      } catch (std::ios_base::failure &e) {}
    }

    if (!rects.size())
      return;

    // Combine all rects into one bouding rect if we detect any overlap.
    ImageUpdateStats stats;
    detectInteresctions(rects, stats);
    rfb::Rect damagedRect = boundingRect(rects);
    handleDamagedRect(damagedRect, stats);
  }

  void Recorder::handleDamagedRect(rfb::Rect &damagedRect,
                                   ImageUpdateStats stats)
  {
    const int width = damagedRect.br.x - damagedRect.tl.x;
    const int height = damagedRect.br.y - damagedRect.tl.y;
    const int x_offset = damagedRect.tl.x;
    const int y_offset = damagedRect.tl.y;

    // Get the damaged region from the display
    ::Image* damagedImage = factory.newImage(dpy, width, height);
    damagedImage->get(DefaultRootWindow(dpy), x_offset, y_offset);
    const uint8_t* data = (uint8_t*) damagedImage->xim->data;

    // Save changed rectangle
    suite::Image* image = decoder->encodeImageToMemory(data,
                                                       width, height,
                                                       x_offset,
                                                       y_offset);
    delete damagedImage;

    // Since encoding times are calculated after an update, we need to keep
    // the previous update in memory until the next one so that the correct
    // stats can be saved.
    if (lastImage == NULL) {
      lastImage = image;
      lastImageStats = stats;
      return;
    }

    suite::ImageUpdate* update = new suite::ImageUpdate(lastImage, lastImageStats);
    fs->addUpdate(update);

    delete update;

    lastImage = image;
    lastImageStats = stats;
  }

  rfb::Rect Recorder::rectFromEvent(XEvent& event)
  {
    if (!(event.type == xdamageEventBase))
      throw std::ios_base::failure("XEvent not DAMAGE event");

    XDamageNotifyEvent* dev;
    rfb::Rect rect;
    // Get damage from window
    dev = (XDamageNotifyEvent*)&event;
    rect.setXYWH(dev->area.x, dev->area.y,
                 dev->area.width, dev->area.height);
    rect = rect.translate(rfb::Point(-geo->offsetLeft(),
                                     -geo->offsetTop()));
    return rect;
  }

  void Recorder::stopRecording()
  {
    throw std::logic_error("method not implemented");
  }
}