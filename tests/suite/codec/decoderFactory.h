#ifndef __DECODER_FACTORY_H__
#define __DECODER_FACTORY_H__
#endif

#include "ImageDecoder.h"

namespace suite {
  ImageDecoder* constructDecoder(decoderEnum decoder);
  ImageDecoder* constructDecoder(std::string decoder);
}
