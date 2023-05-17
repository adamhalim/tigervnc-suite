#include "Recorder.h"
#include "../../unix/x0vncserver/XPixelBuffer.h"
#include "tx/TXWindow.h"
#include "x0vncserver/Image.h"
#include <X11/Xlib.h>
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
    rfb::Rect damagedRect;
    for (XEvent event : events) {
      if (event.type == xdamageEventBase) {
        damagedRect = damagedRect.union_boundary(rectFromEvent(event));
      }
    }

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
    suite::ImageUpdate* update = new suite::ImageUpdate(image);
    fs->addUpdate(update);
    delete update;
    delete damagedImage;
  }

  rfb::Rect Recorder::rectFromEvent(XEvent& event)
  {
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
