#ifndef __SUITE_BRUTEFORCE_H__
#define __SUITE_BRUTEFORCE_H__

#include "../benchmark/Benchmark.h"
#include "TimedBruteForceEncoder.h"
#include <rfb/EncodeManager.h>
#include <rfb/SConnection.h>
#include <string>

namespace suite {

  /*
    This class is used to calculate what the upper bound limit for compression
    ratio is for a given Manager. It will use a brute-force approach to 
    find the encoder with the highest compression for each rectangle that is
    encdoded.
  */
  class BruteForce : public Benchmark
  {
  public:
    BruteForce(std::string filename);
    BruteForce(std::string filename, Server* server, Manager* manager);
    ~BruteForce();

    void run();
    void setBruteForceEncoder(TimedBruteForceEncoder* encoder); 
  private:
    void setDefaultBruteForceEncoder();
    Server* server;
    Manager* manager;
    TimedBruteForceEncoder* bruteForceEncoder;
  };
}
#endif // __SUITE_BRUTEFORCE_H__
