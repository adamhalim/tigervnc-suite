#include "TimedEncoder.h"
#include "rdr/MemOutStream.h"
#include "rfb/PixelBuffer.h"
#include "rfb/SConnection.h"
#include <chrono>

namespace suite {

  TimedEncoder::TimedEncoder(std::string name)
  {
    stats_ = encoderStats{ 
      .writeRectEncodetime = 0,
      .writeSolidRectEncodetime = 0,
      .inputSize = 0,
      .outputSize = 0,
      .name = name,
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
    stats_.inputSize += pb->width() * pb->height() * 4;
    stats_.outputSize += encoderOutstream->length();

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
    stats_.inputSize += width * height * 4;
    stats_.outputSize += encoderOutstream->length();

    // Return the original OutStream after encoding
    // & reset our MemOutStream
    conn_->setStreams(is, os);
    encoderOutstream->clear();
  }
}