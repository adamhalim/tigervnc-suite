#include "TimedEncoder.h"
#include <chrono>

namespace suite {

  TimedEncoder::TimedEncoder()
  {
    stats_ = encoderStats{};
  }

  TimedEncoder::~TimedEncoder()
  {
  }

  void TimedEncoder::startWriteRectTimer()
  {
    writeRectStart = std::chrono::system_clock::now();
  }

  void TimedEncoder::stopWriteRectTimer()
  {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> time = now - writeRectStart;
    stats_.writeRectEncodetime += time.count();
  }

  void TimedEncoder::startWriteSolidRectTimer()
  {
    writeSolidRectStart = std::chrono::system_clock::now();
  }

  void TimedEncoder::stopWriteSolidRectTimer()
  {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> time = now - writeSolidRectStart;
    stats_.writeSolidRectEncodetime += time.count();
  }
}