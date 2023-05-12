#include "timedEncoderFactory.h"
#include "TimedRawEncoder.h"
#include "TimedRREEncoder.h"
#include "TimedHextileEncoder.h"
#include "TimedTightEncoder.h"
#include "TimedTightJPEGEncoder.h"
#include "TimedZRLEEncoder.h"
#include "rfb/SConnection.h"
#include <stdexcept>

namespace suite {
  TimedEncoder* constructTimedEncoder(EncoderClass encoder,
                                      rfb::SConnection* sconn)
  {
    switch(encoder) {
    case encoderRaw:
      return new TimedRawEncoder(sconn);
    case encoderRRE:
      return new TimedRREEncoder(sconn);
    case encoderHextile:
      return new TimedHextileEncoder(sconn);
    case encoderTight:
      return new TimedTightEncoder(sconn);
    case encoderTightJPEG:
      return new TimedTightJPEGEncoder(sconn);
    case encoderZRLE:
      return new TimedZRLEEncoder(sconn);
    default:
      throw std::logic_error("decoder not implemented");
    }
  }
}
