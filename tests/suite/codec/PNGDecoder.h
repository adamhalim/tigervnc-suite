#ifndef __SUITE_PNG_DECODER_H__
#define __SUITE_PNG_DECODER_H__

#include "ImageDecoder.h"

namespace suite {
  class PNGDecoder : public ImageDecoder
  {
  public:
    PNGDecoder();
    ~PNGDecoder();

    Image *decodeImageFromFile(std::string filename);
    Image* decodeImageFromMemory(uint8_t* data, int width, int height,
                                 int size,
                                 int x_offset = 0,
                                 int y_offset = 0);

    void encodeImageTofile(const uint8_t* data, int width,
                           int height, std::string filename);

    Image* encodeImageToMemory(const uint8_t* data, int width, int height,
                               int x_offset = 0, int y_offset = 0);
  private:
    // Sets all bits in alpha channel to 255
    uint8_t* addAlphaPadding(const uint8_t* data, int width, int height);
  };
}

#endif // __SUITE_PNG_DECODER_H__
