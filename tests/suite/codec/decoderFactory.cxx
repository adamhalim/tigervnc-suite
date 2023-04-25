#include "decoderFactory.h"
#include "PNGDecoder.h"
#include "PPMDecoder.h"
#include "QOIDecoder.h"

namespace suite {
  ImageDecoder* constructDecoder(decoderEnum decoder)
  {
  switch (decoder) {
    case PPM:
      return (ImageDecoder*)new PPMDecoder();
    case PNG:
      return (ImageDecoder*)new PNGDecoder();
    break;
    case QOI:
      return (ImageDecoder*)new QOIDecoder();
    break;
    default:
      throw std::logic_error("decoder not imlpemented");
    }
  }

  ImageDecoder* constructDecoder(std::string decoder)
  {
    return constructDecoder(decodersMap[decoder]);
  }
}