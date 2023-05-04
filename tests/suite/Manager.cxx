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

    for(Encoder* e : encoders) {
      TimedEncoder* timedEncoder = dynamic_cast<TimedEncoder*>(e);
      assert(timedEncoder != NULL);
      timedEncoders.push_back(timedEncoder);
    }
  }

  Manager::~Manager()
  {
  }

  std::vector<encoderStats> Manager::stats() {
    std::vector<encoderStats> stats;
    for(TimedEncoder* encoder : timedEncoders) {
      if (encoder->stats().writeRectEncodetime ||
          encoder->stats().writeSolidRectEncodetime)
        stats.push_back(encoder->stats());  
    }
    return stats;
  }
}
