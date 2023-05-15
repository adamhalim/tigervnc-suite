#include "TimedEncoder.h"
#include "encoderStats.h"
#include "rdr/MemOutStream.h"
#include "rfb/PixelBuffer.h"
#include "rfb/SConnection.h"
#include "../../Manager.h"
#include "../../Server.h"
#include <chrono>

namespace suite {

  TimedEncoder::TimedEncoder(EncoderClass encoderclass) 
                           : encoderClass(encoderclass),
                             currentWriteUpdate(0)
  {
    stats_ = encoderStats{ 
      .writeRectEncodetime = 0,
      .writeSolidRectEncodetime = 0,
      .inputSizeRects = 0,
      .outputSizeRects = 0,
      .inputSizeSolidRects = 0,
      .outputSizeSolidRects = 0,
      .nRects = 0,
      .nSolidRects = 0,
      .delayedFrames = 0,
      .name = encoderClassName(encoderclass),
      };
    // Use an OutStream and inject it to the underlying 
    // SConn just before writeRect() and writeSolidRect()
    // so that we can keep track of how much data is written
    // by each encoder (excluding RFB protocol overheads)
    encoderOutstream = new rdr::MemOutStream(10 << 20);
  }

  TimedEncoder::~TimedEncoder()
  {
    delete encoderOutstream;
  }

  void TimedEncoder::startWriteRectTimer(rfb::SConnection* sconn)
  {
    conn_ = sconn;
    os = conn_->getOutStream();
    is = conn_->getInStream();
    // Inject our own MemOutStream just before encoding occurs
    conn_->setStreams(is, encoderOutstream);
    writeRectStart = std::chrono::system_clock::now();
  }

  void TimedEncoder::stopWriteRectTimer(const rfb::PixelBuffer* pb)
  {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> time = now - writeRectStart;

    stats_.writeRectEncodetime += time.count();
    stats_.inputSizeRects += pb->width() * pb->height() * 4;
    stats_.outputSizeRects += encoderOutstream->length();
    stats_.nRects++;

    // Keep track of rects belonging to the same writeUpdate().
    writeRectStats stats {
      .timeSpent = time.count(),
      .pixelCount = pb->width() * pb->height(),
    };
    stats_.writeUpdates[currentWriteUpdate].writeRects.push_back(stats);

    // Return the original OutStream after encoding
    // & reset our MemOutStream
    conn_->setStreams(is, os);
    encoderOutstream->clear();
  }

  void TimedEncoder::startWriteSolidRectTimer(rfb::SConnection* sconn)
  {
    conn_ = sconn;
    os = conn_->getOutStream();
    is = conn_->getInStream();
    // Inject our own MemOutStream just before encoding occurs
    conn_->setStreams(is, encoderOutstream);
    writeSolidRectStart = std::chrono::system_clock::now();
  }

  void TimedEncoder::stopWriteSolidRectTimer(int width, int height)
  {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> time = now - writeSolidRectStart;

    stats_.writeSolidRectEncodetime += time.count();
    stats_.inputSizeSolidRects += width * height * 4;
    stats_.outputSizeSolidRects += encoderOutstream->length();
    stats_.nSolidRects++;

    // Keep track of rects belonging to the same writeUpdate().
    writeRectStats stats {
      .timeSpent = time.count(),
      .pixelCount = width * height,
    };
    stats_.writeUpdates[currentWriteUpdate].writeSolidRects.push_back(stats);

    // Return the original OutStream after encoding
    // & reset our MemOutStream
    conn_->setStreams(is, os);
    encoderOutstream->clear();
  }

  void TimedEncoder::addWriteUpdate(writeUpdateStats data)
  {
    stats_.delayedFrames += !data.encodedInTime();
    stats_.framesData.push_back(data);
  }
}