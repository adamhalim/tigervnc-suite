#ifndef __UPDATES_IN_STREAM_H__
#define __UPDATES_IN_STREAM_H__

#include "../codec/ImageDecoder.h"
namespace suite {
  // This class parses an istream that contains data following the structure
  // defined in FrameOutStream.h
  class FrameInStream
  {
  public:
    FrameInStream(ImageDecoder* decoder);
    ~FrameInStream();

    // Parses one image from the isteam.
    // Should be called repeatedly in a loop to parse an entire file.
    Image* readImage(std::istream& is);
  protected:
    ImageDecoder* decoder;
  };
}

#endif

