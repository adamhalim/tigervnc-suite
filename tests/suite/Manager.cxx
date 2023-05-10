#include "Manager.h"
#include "codec/timed/TimedEncoder.h"
#include "codec/timed/TimedHextileEncoder.h"
#include "codec/timed/TimedRREEncoder.h"
#include "codec/timed/TimedTightEncoder.h"
#include "codec/timed/TimedRawEncoder.h"
#include "codec/timed/TimedTightJPEGEncoder.h"
#include "codec/timed/TimedZRLEEncoder.h"
#include "codec/timed/timedEncoderFactory.h"
#include "rfb/EncodeManager.h"
#include "rfb/Encoder.h"
#include "rfb/SConnection.h"
#include "rfb/encodings.h"
#include <algorithm>
#include <cassert>

namespace suite {

  Manager::Manager(rfb::SConnection *conn) : EncodeManager(conn),
                                             SINGLE_ENCODER(false)
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

  Manager::Manager(rfb::SConnection* conn, EncoderSettings settings) 
                                         : EncodeManager(conn),
                                           SINGLE_ENCODER(true)
  {
    for (Encoder* e : encoders) {
      delete e;
    }

    EncoderClass encoderClass = settings.encoderClass;
    TimedEncoder* timedEncoder = constructTimedEncoder(encoderClass, conn);
    timedEncoders[encoderClass] = timedEncoder;
    for (uint i = 0; i < ENCODERS_COUNT; i++) {
      encoders[static_cast<EncoderClass>(i)] = 
               dynamic_cast<Encoder*>(timedEncoder);
    }
  }

  Manager::~Manager()
  {
    // Ugly hack, EncodeManager's destructor will call delete on all
    // elements in encoders. Since we sometimes use the same pointer for
    // all elements in encoders, we need to make sure there is something
    // to be freed. 
    if (SINGLE_ENCODER) {
      for (uint i = 1; i < ENCODERS_COUNT; i++) {
        encoders[i] = dynamic_cast<Encoder*>
                      (constructTimedEncoder(encoderRaw, conn));
      }
    }
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
