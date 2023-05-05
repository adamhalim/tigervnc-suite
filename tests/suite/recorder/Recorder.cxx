#include "Recorder.h"
#include "../../unix/x0vncserver/XPixelBuffer.h"
#include "tx/TXWindow.h"
#include <X11/Xlib.h>

namespace suite {

  Recorder::Recorder(std::string filename, ImageDecoder* decoder,
                    std::string display) : decoder(decoder)
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
    if(!XDamageQueryExtension(dpy, &xdamageEventBase, &xdamageErrorBase)) {
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
    ImageFactory factory(false);
    rfb::PixelBuffer* pb = new XPixelBuffer(dpy, factory, geo->getRect());

    try {
      fs->writeHeader(pb->width(), pb->height());
    } catch (std::logic_error &e) {
      // FIXME: Handle cases where startRecording() is called twice?
      throw;
    }
    delete pb;

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
    }
  }

  void Recorder::handleEvents(std::vector<XEvent>& events)
  {
    rfb::Rect damagedRect;
    for(XEvent event : events) {
      if (event.type == xdamageEventBase) {
        XDamageNotifyEvent* dev;
        rfb::Rect rect;
        // Get damage from window
        dev = (XDamageNotifyEvent*)&event;
        rect.setXYWH(dev->area.x, dev->area.y,
                    dev->area.width, dev->area.height);
        rect = rect.translate(rfb::Point(-geo->offsetLeft(),
                                         -geo->offsetTop()));
        damagedRect = damagedRect.union_boundary(rect);
      }
    }

    ImageFactory factory(false);
    rfb::PixelBuffer* pb = new XPixelBuffer(dpy, factory, damagedRect);

    // Get framebuffer for damaged rectangle
    int stride;
    const rdr::U8* data = pb->getBuffer(pb->getRect(), &stride);
    int width = damagedRect.br.x - damagedRect.tl.x;
    int height = damagedRect.br.y - damagedRect.tl.y;

    // Save changed rectangle
    suite::Image* image = decoder->encodeImageToMemory(data, 
                                                        width, height,
                                                        damagedRect.tl.x,
                                                        damagedRect.tl.y);
    suite::ImageUpdate* update = new suite::ImageUpdate(image);
    fs->addUpdate(update);
    delete update;
    delete pb;
  }

  void Recorder::stopRecording()
  {
    throw std::logic_error("method not implemented");
  }
}