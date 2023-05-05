#ifndef __SUITE_TIMEDENCODER_H__
#define __SUITE_TIMEDENCODER_H__

#include "rdr/InStream.h"
#include "rdr/MemOutStream.h"
#include "rdr/OutStream.h"
#include "rfb/Encoder.h"
#include "rfb/PixelBuffer.h"
#include "rfb/SConnection.h"
#include <chrono>
#include <string>
namespace suite {

  struct encoderStats {
    double writeRectEncodetime;
    double writeSolidRectEncodetime;
    int inputSize;
    int outputSize;
    std::string name;
  };

  class TimedEncoder 
  {
  public:
    TimedEncoder(std::string name);
    ~TimedEncoder();

    void startWriteRectTimer(rfb::SConnection* sconn);
    void stopWriteRectTimer(const rfb::PixelBuffer* pb);
    void startWriteSolidRectTimer(rfb::SConnection* sconn);
    void stopWriteSolidRectTimer(int width, int height);
    encoderStats stats() { return stats_; };
  protected:
    rdr::MemOutStream *encoderOutstream;
    rdr::OutStream* os;
    rdr::InStream* is;
    rfb::SConnection* conn_;
  private:
    encoderStats stats_;
    std::chrono::system_clock::time_point writeRectStart;
    std::chrono::system_clock::time_point writeSolidRectStart;
  };
}

#endif // __SUITE_TIMEDENCODER_H__ 
