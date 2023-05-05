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
    Image* decodeImageFromMemory(rdr::U8* data, int width, int height,
                                          int size,
                                          int x_offset = 0,
                                          int y_offset = 0);

    void encodeImageTofile(const rdr::U8* data, int width,
                            int height, std::string filename);

    Image* encodeImageToMemory(const rdr::U8* data, int width, int height,
                                int x_offset = 0, int y_offset = 0);
  private:
    // Sets all bits in alpha channel to 255
    rdr::U8* addAlphaPadding(const rdr::U8* data, int width, int height);
  };
}


#endif // __SUITE_PNG_DECODER_H__
