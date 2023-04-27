#ifndef PPM_DECODER_H
#define PPM_DECODER_H

#include "ImageDecoder.h"

namespace suite {
  class PPMDecoder : public ImageDecoder
  {
  public:
    PPMDecoder();
    ~PPMDecoder();

    Image *decodeImageFromFile(std::string filename);
    void encodeImageTofile(Image *image, std::string filename);
    void encodeImageTofile(const rdr::U8* data, int width, int height,
                           std::string filename);

    Image* decodeImageFromMemory(rdr::U8* data, int width, int height,int size,
                                 int x_offset = 0, int y_offset = 0);

    Image* encodeImageToMemory(const rdr::U8* data, int width, int height,
                               int offset_x = 0, int offset_y = 0);

  };
}

#endif