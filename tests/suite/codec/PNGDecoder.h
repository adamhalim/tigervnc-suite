#ifndef __PNG_DECODER_H__
#define __PNG_DECODER_H__

#include "ImageDecoder.h"

class PNGDecoder : public ImageDecoder
{
public:
  PNGDecoder();
  ~PNGDecoder();

  Image *decodeImage(std::string filename);
  void encodeImage(const rdr::U8* data, int width, int height, std::string filename);
private:
  // Sets all bits in alpha channel to 255
  rdr::U8* addAlphaPadding(const rdr::U8* data, int width, int height);
};

#endif