#include "Recorder.h"
#include "util.h"
#include "tx/TXWindow.h"
#include "x0vncserver/Image.h"
#include <X11/Xlib.h>
#include <ios>
#include <sys/select.h>
#include <chrono>
#include <thread>

namespace suite {

  Recorder::Recorder(std::string filename, ImageDecoder* decoder,
                     std::string display) : factory(false), decoder(decoder)
  {
    // XOpenDisplay takes ownership of display string,
    // so we need to make a copy.
    char* displayCopy = new char[display.length() + 1];
    strncpy(displayCopy, display.c_str(), display.length());
    displayCopy[display.length()] = '\0';

    rfb::CharArray dpyStr(displayCopy);
    if (!(dpy = XOpenDisplay(dpyStr.buf[0] ? dpyStr.buf : 0))) {
      std::cerr << "unable to open display " << XDisplayName(dpyStr.buf)
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
      fs->writeHeader(geo->width(), geo->height());
    } catch (std::logic_error &e) {
      // FIXME: Handle cases where startRecording() is called twice?
      throw;
    }

    // FIXME: stopRecording() should stop the loop
    while (true) {
      std::vector<XEvent> events;
      while (XPending(dpy)) {
        XEvent event;
        XNextEvent(dpy, &event);
        events.push_back(event);
      }
      if (events.size()) {
        handleEvents(events);
      }

        // Sleep to reduce CPU usage when idle
        struct timespec timespec;
        timespec.tv_sec = 0;
        timespec.tv_nsec = 1;
        if (pselect(0, 0, 0, 0, &timespec, 0) < 0)
          std::abort();
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
    IntersectionStats stats = detectInteresctions(rects);
    rfb::Rect damagedRect = boundingRect(rects);
    handleDamagedRect(damagedRect, stats);
  }

  void Recorder::handleDamagedRect(rfb::Rect &damagedRect,
                                   IntersectionStats stats)
  {
    const int width = damagedRect.br.x - damagedRect.tl.x;
    const int height = damagedRect.br.y - damagedRect.tl.y;
    const int x_offset = damagedRect.tl.x;
    const int y_offset = damagedRect.tl.y;

    // Get the damaged region from the display
    ::Image* damagedImage = factory.newImage(dpy, width, height);
    damagedImage->get(DefaultRootWindow(dpy), x_offset, y_offset);
    const rdr::U8* data = (rdr::U8*) damagedImage->xim->data;

    // Save changed rectangle
    suite::Image* image = decoder->encodeImageToMemory(data, 
                                                       width, height,
                                                       x_offset,
                                                       y_offset);
    suite::ImageUpdate* update = new suite::ImageUpdate(image, stats);
    fs->addUpdate(update);
    delete update;
    delete damagedImage;
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
