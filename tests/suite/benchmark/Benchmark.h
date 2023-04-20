#ifndef __BENCHMARK_H__
#define __BENCHMARK_H__

#include "../codec/ImageDecoder.h"
#include "../Server.h"
#include <chrono>

namespace suite {

  static const std::string OFFSETS_FILENAME = "offsets.txt";

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
    Benchmark(std::string dirName, ImageDecoder* decoder);
    Benchmark(std::string dirName, ImageDecoder* decoder, 
              std::vector<std::pair<double, double> > offsets);
    ~Benchmark();

    // Runs decoding benchmark on the server.
    // Throws exception if offsets don't line up with files
    void runBenchmark(Server* server);
    double getTime();

  protected:

    std::vector<std::string> files; 
    suite::stats stats;
    std::string dirName;
    ImageDecoder* decoder;
    std::vector<std::pair<double, double> > offsets;
  };

  #endif
}