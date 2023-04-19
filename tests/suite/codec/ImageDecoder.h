#ifndef __IMAGE_DECODER_H__
#define __IMAGE_DECODER_H__

#include "Image.h"
class ImageDecoder
{
public:
  ImageDecoder() {};
  virtual ~ImageDecoder() {};

  virtual Image *decodeImage(std::string filename) = 0;
};
#endif