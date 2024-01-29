#ifndef __SUITE_BENCHMARK_H__
#define __SUITE_BENCHMARK_H__

#include "../codec/ImageDecoder.h"
#include "../Server.h"
#include "../io/FrameInStream.h"
#include <chrono>

namespace suite {

  class Benchmark
  {
  public:
    Benchmark(std::string filename, const rdr::S32* e = encodings,
                                    const size_t len = sizeof(encodings)
                                                     / sizeof(*encodings));
    ~Benchmark();

    // Runs decoding benchmark on the server.
    void runBenchmark(EncoderSettings* settings, size_t len);
    // Default benchmark runs each encoder once (ENCODERS_COUNT in total)
    void runBenchmark();
    int width() const { return width_; }
    int height() const { return height_; }
    Server* server() const { return defaultServer_; }
    #ifdef _DEBUG
    Server* debugServer_;
    #endif //_DEBUG

  protected:
    std::string filename;
    Server* defaultServer_;
    int width_;
    int height_;
  private:
    const rdr::S32* encodings_;
    const size_t encodingsLength_;
    static EncoderSettings encoderSettings(EncoderClass encoderClass,
                                        PseudoEncodingLevel quality = NONE,
                                        PseudoEncodingLevel compression = TWO);
  };
}
#endif // __SUITE_BENCHMARK_H__
