#ifndef __SUITE_TIMED_BRUTEFORCE_H__
#define __SUITE_TIMED_BRUTEFORCE_H__

#include "../codec/timed/TimedEncoder.h"
#include <rfb/RawEncoder.h>
#include <rfb/SConnection.h>

namespace suite {
  using namespace rfb;

  struct BestEncoder {
    enumEncoder::EncoderClass encoder;
    int inputSize;
    uint outputSize;
    double encodingTime;
  };

  class TimedBruteForceEncoder : public RawEncoder, public TimedEncoder
  {
  public:
    TimedBruteForceEncoder(SConnection* sconn,
                           std::vector<TimedEncoder*> encoders);
    ~TimedBruteForceEncoder();

    virtual void writeRect(const PixelBuffer* pb, const Palette& palette);
    virtual void writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour);
    void bestEncoder(double time, uint inputSize, uint outputSize,
                     EncoderClass encoder, BestEncoder* best);

    std::vector<BestEncoder> bestEncoders() { return bestEncoders_; }
    std::map<EncoderClass, int> bestCounter() { return bestCounter_; }
  private:
    std::vector<TimedEncoder*> encoders;
    std::vector<BestEncoder> bestEncoders_;
    std::map<EncoderClass, int> bestCounter_;
  };
}

#endif // __SUITE_TIMED_BRUTEFORCE_H__
