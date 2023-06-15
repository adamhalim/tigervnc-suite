#include "timedEncoderFactory.h"
#include "TimedRawEncoder.h"
#include "TimedRREEncoder.h"
#include "TimedHextileEncoder.h"
#include "TimedTightEncoder.h"
#include "TimedTightJPEGEncoder.h"
#include "TimedZRLEEncoder.h"
#include <rfb/SConnection.h>
#include <stdexcept>

namespace suite {
  TimedEncoder* constructTimedEncoder(EncoderClass encoder,
                                      rfb::SConnection* sconn,
                                      Manager* manager)
  {
    switch(encoder) {
    case encoderRaw:
      return new TimedRawEncoder(sconn, manager);
    case encoderRRE:
      return new TimedRREEncoder(sconn, manager);
    case encoderHextile:
      return new TimedHextileEncoder(sconn, manager);
    case encoderTight:
      return new TimedTightEncoder(sconn, manager);
    case encoderTightJPEG:
      return new TimedTightJPEGEncoder(sconn, manager);
    case encoderZRLE:
      return new TimedZRLEEncoder(sconn, manager);
    default:
      throw std::logic_error("decoder not implemented");
    }
  }
}
