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
#include <iostream>

namespace suite {

  Manager::Manager(rfb::SConnection *conn) : EncodeManager(conn),
                                             SINGLE_ENCODER(false)
  {
    for (Encoder* encoder : encoders)
      delete encoder;

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
    for (Encoder* e : encoders)
      delete e;

    EncoderClass encoderClass = settings.encoderClass;
    TimedEncoder* timedEncoder = constructTimedEncoder(encoderClass, conn);
    timedEncoders[encoderClass] = timedEncoder;
    for (uint i = 0; i < ENCODERS_COUNT; i++) {
      encoders[static_cast<EncoderClass>(i)] = 
               dynamic_cast<Encoder*>(timedEncoder);
    }
    timedEncoder_ = timedEncoder;
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

  void Manager::writeUpdate(const rfb::UpdateInfo& ui,
                            const rfb::PixelBuffer* pb,
                            const rfb::RenderedCursor* renderedCursor)
  {
    EncodeManager::writeUpdate(ui, pb, renderedCursor);
  }

  void Manager::writeUpdate(const rfb::UpdateInfo& ui,
                            const rfb::PixelBuffer* pb,
                            const rfb::RenderedCursor* renderedCursor,
                            uint frameTime)
  {
    auto start = std::chrono::system_clock::now();
    writeUpdate(ui, pb, renderedCursor);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> time = end - start;

    // We keep track of the time it takes to encode an entire frame,
    // and how much time there is left until the next frame occurs
    // (as it was recorded).
    writeUpdateStats data {
      .timeRequired = frameTime,
      .timeSpent = (uint) (time.count() * 10e2),
    };
    timedEncoder_->addWriteUpdate(data);
  }

  std::map<EncoderClass, encoderStats> Manager::stats() {
    std::map<EncoderClass, encoderStats> stats;
    for (auto const& encoder : timedEncoders) {
      if (encoder.second->stats().writeRectEncodetime ||
          encoder.second->stats().writeSolidRectEncodetime)
          stats[encoder.first] = encoder.second->stats();
    }
    return stats;
  }
}
