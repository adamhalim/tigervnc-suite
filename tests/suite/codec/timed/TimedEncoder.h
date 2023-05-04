#ifndef __SUITE_TIMEDENCODER_H__
#define __SUITE_TIMEDENCODER_H__

#include "rfb/Encoder.h"
#include <chrono>
namespace suite {

  struct encoderStats {
    double writeRectEncodetime;
    double writeSolidRectEncodetime;
  };

  class TimedEncoder 
  {
  public:
    TimedEncoder();
    ~TimedEncoder();

    void startWriteRectTimer();
    void stopWriteRectTimer();
    void startWriteSolidRectTimer();
    void stopWriteSolidRectTimer();
    encoderStats stats() { return stats_; };
  private:
    encoderStats stats_;
    std::chrono::system_clock::time_point writeRectStart;
    std::chrono::system_clock::time_point writeSolidRectStart;
  };
}

#endif // __SUITE_TIMEDENCODER_H__ 
