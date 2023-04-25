#ifndef __UPDATES_IN_STREAM_H__
#define __UPDATES_IN_STREAM_H__

#include "../codec/ImageDecoder.h"
namespace suite {
  // This class parses an istream that contains data following the structure
  // defined in FrameOutStream.h
  //
  // FIXME: Probably want to wrap this class so that it is more seamless to
  // use. We shouldn't have to call parseHeader() before readImage().
  class FrameInStream
  {
  public:
    FrameInStream();
    ~FrameInStream();

    // Parses one image from the isteam.
    // Should be called repeatedly in a loop to parse an entire file.
    Image* readImage(std::istream& is);
    // Parses the header of the file and returns a pair with the 
    // width and size of the framebuffer. 
    // Needs to be run before readImage as it also sets the decoder.
    std::pair<int, int> parseHeader(std::istream& is);
  protected:
    ImageDecoder* decoder;
  private:
    bool headerParsed;
  };
}

#endif

