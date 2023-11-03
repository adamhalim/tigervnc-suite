#include "timedEncoderFactory.h"
#include "TimedEncoder.h"
#include "rfb/Encoder.h"
#include "rfb/HextileEncoder.h"
#include "rfb/RREEncoder.h"
#include "rfb/RawEncoder.h"
#include "rfb/TightEncoder.h"
#include "rfb/TightJPEGEncoder.h"
#include "rfb/ZRLEEncoder.h"
#include <rfb/SConnection.h>
#include <stdexcept>

namespace suite {

  TimedEncoder* constructTimedEncoder(EncoderClass encoder,
                                      rfb::SConnection* sconn)
  {
    rfb::Encoder* e;
    struct EncoderArgs s;
    switch(encoder) {
    case encoderRaw:
      e = new rfb::RawEncoder(sconn);
      s = EncoderArgs{e->encoding, e->flags, e->maxPaletteSize, e->losslessQuality};
      break;
    case encoderRRE:
      e = new rfb::RREEncoder(sconn);
      s = EncoderArgs{e->encoding, e->flags, e->maxPaletteSize, e->losslessQuality};
      break;
    case encoderHextile:
      e = new rfb::HextileEncoder(sconn);
      s = EncoderArgs{e->encoding, e->flags, e->maxPaletteSize, e->losslessQuality};
      break;
    case encoderTight:
      e = new rfb::TightEncoder(sconn);
      s = EncoderArgs{e->encoding, e->flags, e->maxPaletteSize, e->losslessQuality};
      break;
    case encoderTightJPEG:
      e = new rfb::TightJPEGEncoder(sconn);
      s = EncoderArgs{e->encoding, e->flags, e->maxPaletteSize, e->losslessQuality};
      break;
    case encoderZRLE:
      e = new rfb::ZRLEEncoder(sconn);
      s = EncoderArgs{e->encoding, e->flags, e->maxPaletteSize, e->losslessQuality};
      break;
    default:
      throw std::logic_error("decoder not implemented");
    }
    return new TimedEncoder(encoder, e, sconn, s);
  }
}
