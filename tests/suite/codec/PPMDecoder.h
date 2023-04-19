#ifndef PPM_DECODER_H
#define PPM_DECODER_H

#include "ImageDecoder.h"

class PPMDecoder : public ImageDecoder
{
public:
  PPMDecoder();
  ~PPMDecoder();

  Image *decodeImage(std::string filename);
  static void encodeImage(Image *image, std::string filename);
  static void encodeImage(const rdr::U8* data, int width, int height, std::string filename);

};

#endif