#include "Manager.h"
#include "codec/timed/TimedEncoder.h"
#include "codec/timed/TimedHextileEncoder.h"
#include "codec/timed/TimedRREEncoder.h"
#include "codec/timed/TimedTightEncoder.h"
#include "codec/timed/TimedRawEncoder.h"
#include "codec/timed/TimedTightJPEGEncoder.h"
#include "codec/timed/TimedZRLEEncoder.h"
#include "rfb/EncodeManager.h"
#include "rfb/Encoder.h"
#include "rfb/SConnection.h"
#include "rfb/encodings.h"
#include <algorithm>
#include <cassert>

namespace suite {

  Manager::Manager(rfb::SConnection *conn) : EncodeManager(conn)
  {
    for(Encoder* encoder : encoders) {
      delete encoder;
    }

    encoders[encoderRaw] = new TimedRawEncoder(conn);
    encoders[encoderRRE] = new TimedRREEncoder(conn);
    encoders[encoderHextile] = new TimedHextileEncoder(conn);
    encoders[encoderTight] = new TimedTightEncoder(conn);
    encoders[encoderTightJPEG] = new TimedTightJPEGEncoder(conn);
    encoders[encoderZRLE] = new TimedZRLEEncoder(conn);


    // Since Tight & TightJPEG share the same encode value (7), 
    // we can't just loop through the encoders. We use EncoderClass as an 
    // identifier instead of Encoder.encoding
    timedEncoders[encoderRaw] =
             dynamic_cast<TimedEncoder*>(encoders[encoderRaw]);
    timedEncoders[encoderRRE] = 
              dynamic_cast<TimedEncoder*>(encoders[encoderRRE]);
    timedEncoders[encoderHextile] =
              dynamic_cast<TimedEncoder*>(encoders[encoderHextile]);
    timedEncoders[encoderTight] =
              dynamic_cast<TimedEncoder*>(encoders[encoderTight]);
    timedEncoders[encoderTightJPEG] = 
              dynamic_cast<TimedEncoder*>(encoders[encoderTightJPEG]);
    timedEncoders[encoderZRLE] =
              dynamic_cast<TimedEncoder*>(encoders[encoderZRLE]);
  }

  Manager::~Manager()
  {
  }

  std::map<const int, encoderStats> Manager::stats() {
    std::map<const int, encoderStats> stats;
    for(auto const& encoder : timedEncoders) {
      if (encoder.second->stats().writeRectEncodetime ||
          encoder.second->stats().writeSolidRectEncodetime)
          stats[encoder.first] = encoder.second->stats();
    }
    return stats;
  }
}
