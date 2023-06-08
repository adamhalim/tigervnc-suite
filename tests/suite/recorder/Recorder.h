#ifndef __SUITE_RECORDER_H__
#define __SUITE_RECORDER_H__

#include "../../unix/x0vncserver/Geometry.h"
#include "../io/FrameOutStream.h"
#include "../../unix/x0vncserver/XPixelBuffer.h"
#include "tx/TXWindow.h"
#include "util.h"
#include <X11/extensions/Xdamage.h>
#include <X11/Xlib.h>
#include <chrono>
#include <string>
#include <vector>

namespace suite {

  /* This class is used to record an X display to file.
  */
  class Recorder
  {
  public:
    Recorder(std::string filename, ImageDecoder* decoder, std::string display,
                                                          int framerate);
    ~Recorder();

    // Starts recording to file
    void startRecording();
    void stopRecording();

    void handleEvents(std::vector<XEvent>& events);
    protected:
    void handleDamagedRect(rfb::Rect &damagedRect, ImageUpdateStats stats);
    rfb::Rect rectFromEvent(XEvent& event);
    Damage damage;
    int xdamageEventBase;

  private:
    Display* dpy;
    FrameOutStream* fs;
    Geometry* geo;
    ImageFactory factory;
    ImageDecoder* decoder;
    const double interval;
    const double intervalThreshold;
  };
}

#endif // __SUITE_RECORDER_H__
