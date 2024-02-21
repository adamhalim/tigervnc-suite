#ifndef __SUITE_QOI_DECODER_H__
#define __SUITE_QOI_DECODER_H__

#include "ImageDecoder.h"

namespace suite {

  class QOIDecoder : public suite::ImageDecoder
  {
  public:
    QOIDecoder();
    ~QOIDecoder();

    Image *decodeImageFromFile(std::string filename);
    Image* decodeImageFromMemory(uint8_t* data, int width, int height,
                                 int size, int x_offest = 0,
                                 int y_offset = 0);

    void encodeImageTofile(const uint8_t* data, int width, int height,
                           std::string filename);

    Image* encodeImageToMemory(const uint8_t* data, int width, int height,
                               int x_offset = 0, int y_offset = 0);
  };
}

#endif // __SUITE_QOI_DECODER_H__
