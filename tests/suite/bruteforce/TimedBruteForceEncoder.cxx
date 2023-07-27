#include "TimedBruteForceEncoder.h"
#include <rfb/Encoder.h>
#include <rfb/PixelBuffer.h>
#include <rfb/RawEncoder.h>
#include <rfb/SConnection.h>
#include <chrono>
#include <vector>

namespace suite {
  TimedBruteForceEncoder::TimedBruteForceEncoder(SConnection* conn_,
                                                 std::vector<TimedEncoder*>
                                                                  encoders)
    : RawEncoder(conn_), TimedEncoder(encoderHextile), 
      encoders(encoders)
  {
  }

  TimedBruteForceEncoder::~TimedBruteForceEncoder()
  {
  }

  void TimedBruteForceEncoder::writeRect(const PixelBuffer* pb,
                                         const Palette& palette)
  {
    BestEncoder best{};
    best.inputSize = pb->area() * 4;

    for (const auto& encoder : encoders) {
      startWriteRectTimer(this->conn);
      encoder->writeRect(pb, palette);
      stopWriteRectTimer(pb);
      EncoderStats* stats = encoder->stats();
      
      double time = stats->writeRectEncodetime 
                  + stats->writeSolidRectEncodetime;
      uint outputSize = stats->outputSizeRects + stats->outputSizeSolidRects;

      bestEncoder(time, pb->area(), outputSize, encoder->encoderClass, &best);

      stats->outputSizeRects = 0;
      stats->outputSizeSolidRects = 0;
      stats->writeRectEncodetime = 0;
      stats->writeSolidRectEncodetime = 0;
    }

    bestEncoders_.push_back(best);
    bestCounter_[best.encoder]++;
  }

    void TimedBruteForceEncoder::writeSolidRect(int width, int height,
                                                const PixelFormat& pf,
                                                const rdr::U8* colour)
  {
    int inputSize = width * height  *4;
    BestEncoder best{};
    best.inputSize = inputSize;

    for (const auto& encoder : encoders) {
      startWriteSolidRectTimer(this->conn);
      encoder->writeSolidRect(width, height, pf, colour);
      stopWriteSolidRectTimer(width, height);
      EncoderStats* stats = encoder->stats();
      
      uint outputSize = stats->outputSizeRects +  stats->outputSizeSolidRects;
      double time = stats->writeRectEncodetime 
                  + stats->writeSolidRectEncodetime;
      bestEncoder(time, inputSize, outputSize, encoder->encoderClass, &best);

      stats->outputSizeRects = 0;
      stats->outputSizeSolidRects = 0;
      stats->writeRectEncodetime = 0;
      stats->writeSolidRectEncodetime = 0;
    }
    bestEncoders_.push_back(best);
    bestCounter_[best.encoder]++;
  }

  void TimedBruteForceEncoder::bestEncoder(double time, uint inputSize,
                                           uint outputSize,
                                           EncoderClass encoder,
                                           BestEncoder* best)
  {
    bool firstComparison = best->outputSize == 0;
    bool bestCompression = best->outputSize > outputSize;

    if (firstComparison || bestCompression) {
      best->outputSize = outputSize;
      best->encoder = encoder;
      best->encodingTime = time;
    }
  }
}
