#include "decoderFactory.h"
#include "JPEGDecoder.h"

namespace suite {
  ImageDecoder* constructDecoder(DecoderEnum decoder)
  {
    switch (decoder) {
    case JPEG:
      return (ImageDecoder*) new JPEGDecoder();
    default:
      throw std::logic_error("decoder not imlpemented");
    }
  }

  ImageDecoder* constructDecoder(std::string decoder)
  {
    return constructDecoder(decodersMap[decoder]);
  }
}
