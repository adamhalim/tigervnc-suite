#ifndef __SUITE_BENCHMARK_H__
#define __SUITE_BENCHMARK_H__

#include "../codec/ImageDecoder.h"
#include "../Server.h"
#include "../io/FrameInStream.h"
#include <chrono>

namespace suite {

  class stats
  {
  public:
    void startClock();
    void stopClock();
    double time();
  private:
    double encodeTime;

    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
  };

  class Benchmark
  {
  public:
    Benchmark(std::string filename);
    ~Benchmark();

    // Runs decoding benchmark on the server.
    void runBenchmark();
    double getTime();
    int width() const { return width_; }
    int height() const { return height_; }
    Server* server() const { return server_; }

  protected:
    suite::stats stats;
    std::string filename;
    Server* server_;
    int width_;
    int height_;
  };
}
#endif