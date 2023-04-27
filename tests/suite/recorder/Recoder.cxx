#include "Recoder.h"
#include "../../unix/x0vncserver/XPixelBuffer.h"
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace suite {

rfb::StringParameter displayname("display", "The X display", "");

  Recoder::Recoder(std::string filename, ImageDecoder* decoder,
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
  }

  Recoder::~Recoder()
  {
    delete fs;
    delete decoder;
  }

  void Recoder::startRecording()
  {
    ImageFactory factory(true);
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
      pb = new XPixelBuffer(dpy, factory, geo->getRect());
      int stride;
      const rdr::U8* data = pb->getBuffer(pb->getRect(), &stride);
      suite::Image* image = decoder->encodeImageToMemory(data, pb->width(), pb->height());
      suite::ImageUpdate* update = new suite::ImageUpdate(image);
      fs->addUpdate(update);
      delete update;
      delete pb;
    }
  }

  void Recoder::stopRecording()
  {
    throw std::logic_error("method not implemented");
  }
}