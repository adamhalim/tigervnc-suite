#ifndef __SUITE_RECORDER_H__
#define __SUITE_RECORDER_H__

#include "../../unix/x0vncserver/Geometry.h"
#include "../io/FrameOutStream.h"
#include "tx/TXWindow.h"
#include <X11/extensions/Xdamage.h>
#include <X11/Xlib.h>
#include <string>

namespace suite {

  /* This class is used to record an X display to file.
  */
  class Recorder : public TXGlobalEventHandler
  {
  public:
    Recorder(std::string filename, ImageDecoder* decoder, std::string display);
    ~Recorder();

    // Starts recording to file
    void startRecording();
    void stopRecording();

    // -=- TXGlobalEventHandler interface
    virtual bool handleGlobalEvent(XEvent* ev);
  protected:
    Damage damage;
    int xdamageEventBase;

  private:
    Display* dpy;
    FrameOutStream* fs;
    Geometry* geo;
    ImageDecoder* decoder;
  };
}

#endif