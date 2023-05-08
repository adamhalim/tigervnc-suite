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

    for (uint i = 0; i < encoders.size(); i++) {
      timedEncoders[static_cast<EncoderClass>(i)] = 
              dynamic_cast<TimedEncoder*>(encoders[i]);
    }
  }

  Manager::Manager(rfb::SConnection* conn, 
                   std::array<rfb::Encoder*, ENCODERS_COUNT> encoders_) 
                 : EncodeManager(conn)
  {
    for (Encoder* e : encoders) {
      delete e;
    }

    std::vector<Encoder*> encoders(std::begin(encoders_), std::end(encoders_));
    this->encoders = encoders;

    for (int i = 0; i < ENCODERS_COUNT; i++) {
      TimedEncoder* timedEncoder = dynamic_cast<TimedEncoder*>(encoders[i]);
      timedEncoders[timedEncoder->encoderClass] = timedEncoder;
    }
  }

  Manager::~Manager()
  {
  }

  std::map<EncoderClass, encoderStats> Manager::stats() {
    std::map<EncoderClass, encoderStats> stats;
    for(auto const& encoder : timedEncoders) {
      if (encoder.second->stats().writeRectEncodetime ||
          encoder.second->stats().writeSolidRectEncodetime)
          stats[encoder.first] = encoder.second->stats();
    }
    return stats;
  }
}
