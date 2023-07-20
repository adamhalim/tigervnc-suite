#include "Manager.h"
#include "codec/timed/TimedEncoder.h"
#include "codec/timed/TimedHextileEncoder.h"
#include "codec/timed/TimedRREEncoder.h"
#include "codec/timed/TimedTightEncoder.h"
#include "codec/timed/TimedRawEncoder.h"
#include "codec/timed/TimedTightJPEGEncoder.h"
#include "codec/timed/TimedZRLEEncoder.h"
#include "codec/timed/timedEncoderFactory.h"
#include <chrono>
#include <rfb/EncodeManager.h>
#include <rfb/Encoder.h>
#include <rfb/SConnection.h>
#include <rfb/encodings.h>
#include "stats/EncoderStats.h"
#include "stats/ManagerStats.h"
#include <algorithm>
#include <cassert>
#include <iostream>

namespace suite {

  Manager::Manager(rfb::SConnection *conn, bool debug) : EncodeManager(conn),
                                                         SINGLE_ENCODER(false)
  {
    if (debug) {
      currentEncoder = new TimedTightEncoder(conn, this);
      stats_.encoders.push_back(currentEncoder);
      return;
    }

    for (Encoder* encoder : encoders)
      delete encoder;

    encoders[encoderRaw] = new TimedRawEncoder(conn, this);
    encoders[encoderRRE] = new TimedRREEncoder(conn, this);
    encoders[encoderHextile] = new TimedHextileEncoder(conn, this);
    encoders[encoderTight] = new TimedTightEncoder(conn, this);
    encoders[encoderTightJPEG] = new TimedTightJPEGEncoder(conn, this);
    encoders[encoderZRLE] = new TimedZRLEEncoder(conn, this);

    for (uint i = 0; i < encoders.size(); i++) {
      TimedEncoder* timedEncoder = dynamic_cast<TimedEncoder*>(encoders[i]);
      stats_.encoders.push_back(timedEncoder);
    }
  }

  Manager::Manager(rfb::SConnection* conn, EncoderSettings settings) 
                                         : EncodeManager(conn),
                                           SINGLE_ENCODER(true)
  {
    for (Encoder* e : encoders)
      delete e;

    EncoderClass encoderClass = settings.encoderClass;
    TimedEncoder* timedEncoder = constructTimedEncoder(encoderClass, conn,
                                                       this);
    stats_.encoders.push_back(timedEncoder);
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
                      (constructTimedEncoder(encoderRaw, conn, this));
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
    auto time = std::chrono::duration<double, std::milli>(end-start);

    // We keep track of the time it takes to encode an entire frame,
    // and how much time there is left until the next frame occurs
    // (as it was recorded).
    WriteUpdate update {
      .timeRequired = frameTime,
      .timeSpent = time.count() * 10e2,
    };
    stats_.addWriteUpdate(update);

    // Increment the currentWriteUpdate index so we can keep track
    // of which writeRect() belongs to which writeUpdate.
    currentEncoder->currentWriteUpdate++;
  }
}
