#include "Recorder.h"
#include "../../unix/x0vncserver/XPixelBuffer.h"
#include "tx/TXWindow.h"
#include <X11/Xlib.h>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace suite {

rfb::StringParameter displayname("display", "The X display", "");

  Recorder::Recorder(std::string filename, ImageDecoder* decoder,
                    std::string display) : decoder(decoder)
  {
    // XOpenDisplay takes ownership of displayname string,
    // so we need to make a copy.
    char* displayCopy = new char[display.length() + 1];
    strncpy(displayCopy, display.c_str(), display.length());
    displayCopy[display.length()] = '\0';

    rfb::CharArray dpyStr(displayCopy);
    if (!(dpy = XOpenDisplay(dpyStr.buf[0] ? dpyStr.buf : 0))) {
      fprintf(stderr,"unable to open display \"%s\"\r\n",
            XDisplayName(dpyStr.buf));
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
    TXWindow::setGlobalEventHandler(this);

    
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
      TXWindow::handleXEvents(dpy);
    }
  }

  void Recorder::stopRecording()
  {
    throw std::logic_error("method not implemented");
  }

  bool Recorder::handleGlobalEvent(XEvent* ev) {
    if (ev->type == xdamageEventBase) {
      XDamageNotifyEvent* dev;
      rfb::Rect rect;

      // Get damage from window
      dev = (XDamageNotifyEvent*)ev;
      rect.setXYWH(dev->area.x, dev->area.y,
                   dev->area.width, dev->area.height);
      rect = rect.translate(rfb::Point(-geo->offsetLeft(),
                                  -geo->offsetTop()));
      ImageFactory factory(false);
      rfb::PixelBuffer* pb = new XPixelBuffer(dpy, factory, rect);

      // Get framebuffer for damaged rectangle
      int stride;
      const rdr::U8* data = pb->getBuffer(pb->getRect(), &stride);
      int width = rect.br.x - rect.tl.x;
      int height = rect.br.y - rect.tl.y;

      // Save changed rectangle
      suite::Image* image = decoder->encodeImageToMemory(data, 
                                                         width, height,
                                                         rect.tl.x, rect.tl.y);
      suite::ImageUpdate* update = new suite::ImageUpdate(image);
      fs->addUpdate(update);
      delete update;
      delete pb;
    }
    return true;
  }

}