#ifndef __SUITE_PPM_DECODER_H__
#define __SUITE_PPM_DECODER_H__

#include "ImageDecoder.h"

namespace suite {
  class PPMDecoder : public ImageDecoder
  {
  public:
    PPMDecoder();
    ~PPMDecoder();

    Image *decodeImageFromFile(std::string filename);
    void encodeImageTofile(Image *image, std::string filename);
    void encodeImageTofile(const uint8_t* data, int width, int height,
                           std::string filename);

    Image* decodeImageFromMemory(uint8_t* data, int width, int height,int size,
                                 int x_offset = 0, int y_offset = 0);

    Image* encodeImageToMemory(const uint8_t* data, int width, int height,
                               int offset_x = 0, int offset_y = 0);

  };
}

#endif // __SUITE_PPM_DECODER_H__
