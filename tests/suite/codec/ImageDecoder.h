#ifndef __IMAGE_DECODER_H__
#define __IMAGE_DECODER_H__

#include "Image.h"

namespace suite {

  class ImageDecoder
  {
  public:
    ImageDecoder() {};
    virtual ~ImageDecoder() {};

    virtual Image *decodeImageFromFile(std::string filename) = 0;
    virtual Image* decodeImageFromMemory(rdr::U8* data, int width, int height,
                                          int size, int x_offest = 0, int y_offset = 0) = 0;
    virtual void encodeImageTofile(const rdr::U8* data, int width, int height, std::string filename) = 0;
    virtual Image* encodeImageToMemory(const rdr::U8* data, int width,
                                      int height, int offset_x = 0,
                                      int offset_y  = 0) = 0;

  };
}
#endif
