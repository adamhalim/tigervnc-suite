#ifndef __RECORDER_H__
#define __RECORDER_H__

#include "../../unix/x0vncserver/Geometry.h"
#include "../io/FrameOutStream.h"
#include <X11/Xlib.h>
#include <string>

namespace suite {

  /* This class is used to record an X display to file.
  */
  class Recoder
  {
  public:
    Recoder(std::string filename, ImageDecoder* decoder, std::string display);
    ~Recoder();

    // Starts recording to file
    void startRecording();
    void stopRecording();

  private:
    Display* dpy;
    FrameOutStream* fs;
    Geometry* geo;
    ImageDecoder* decoder;
  };
}

#endif