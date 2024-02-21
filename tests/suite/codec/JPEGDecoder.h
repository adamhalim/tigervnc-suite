#ifndef __SUITE_JPEG_DECODER_H__
#define __SUITE_JPEG_DECODER_H__

#include "ImageDecoder.h"
#include <rfb/JpegCompressor.h>
#include <rfb/JpegDecompressor.h>
#include <rfb/TightDecoder.h>

namespace suite {

  class JPEGDecoder : public ImageDecoder
  {
  public:
    JPEGDecoder(int quality = 100, int subsampling = 0);
    ~JPEGDecoder();

    Image *decodeImageFromFile(std::string filename);
    Image* decodeImageFromMemory(uint8_t* data, int width, int height,
                                 int size, int x_offest = 0,
                                 int y_offset = 0);

    void encodeImageTofile(const uint8_t* data, int width, int height,
                           std::string filename);

    Image* encodeImageToMemory(const uint8_t* data, int width, int height,
                               int x_offset = 0, int y_offset = 0);
  private:
    int quality;
    int subsampling;
  };
}

#endif // __SUITE_JPEG_DECODER_H__
