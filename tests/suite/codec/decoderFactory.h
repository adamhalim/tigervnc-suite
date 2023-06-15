#ifndef __SUITE_DECODER_FACTORY_H__
#define __SUITE_DECODER_FACTORY_H__

#include "ImageDecoder.h"

namespace suite {
  ImageDecoder* constructDecoder(DecoderEnum decoder);
  ImageDecoder* constructDecoder(std::string decoder);
}

#endif // __SUITE_DECODER_FACTORY_H__