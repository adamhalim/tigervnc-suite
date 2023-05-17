#ifndef __SUITE_RECORDER_H__
#define __SUITE_RECORDER_H__

#include "../../unix/x0vncserver/Geometry.h"
#include "../io/FrameOutStream.h"
#include "tx/TXWindow.h"
#include <X11/extensions/Xdamage.h>
#include <X11/Xlib.h>
#include <string>
#include <vector>

namespace suite {

  /* This class is used to record an X display to file.
  */
  class Recorder
  {
  public:
    Recorder(std::string filename, ImageDecoder* decoder, std::string display);
    ~Recorder();

    // Starts recording to file
    void startRecording();
    void stopRecording();

    void handleEvents(std::vector<XEvent>& events);
    protected:
    rfb::Rect rectFromEvent(XEvent& event);
    rfb::Rect boundingRect(std::vector<rfb::Rect>& rects);
    Damage damage;
    int xdamageEventBase;

  private:
    Display* dpy;
    FrameOutStream* fs;
    Geometry* geo;
    ImageFactory factory;
    ImageDecoder* decoder;
  };
}

#endif // __SUITE_RECORDER_H__
