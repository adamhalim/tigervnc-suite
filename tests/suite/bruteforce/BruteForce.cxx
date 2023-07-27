#include "../codec/timed/TimedRawEncoder.h"
#include "../codec/timed/TimedRREEncoder.h"
#include "../codec/timed/TimedHextileEncoder.h"
#include "../codec/timed/TimedTightEncoder.h"
#include "../codec/timed/TimedTightJPEGEncoder.h"
#include "../codec/timed/TimedZRLEEncoder.h"
#include "BruteForce.h"
#include "TimedBruteForceEncoder.h"
#include <rfb/TightEncoder.h>
#include <rfb/TightJPEGEncoder.h>
#include <rfb/ZRLEEncoder.h>
#include <rfb/encodings.h>
#include <fstream>

namespace suite {

  BruteForce::BruteForce(std::string filename) 
    : Benchmark(filename), server(defaultServer_),
      manager(defaultServer_->manager)
  {
    setDefaultBruteForceEncoder();
  }

  BruteForce::BruteForce(std::string filename, Server* server,
                                               Manager* manager)
    : Benchmark(filename), server(server), manager(manager)
  {
    setDefaultBruteForceEncoder();
  }

  BruteForce::~BruteForce()
  {
  }

  void BruteForce::run()
  {
    FrameInStream is =  FrameInStream();
    std::ifstream file(filename);
    is.parseHeader(file); // FIXME: Don't parse header twice

    std::cout << "Starting brute-force benchmark using \"" 
              << filename << "\"\n";

    RecorderStats recorderStats;  // unused
    while (file.peek() != EOF) {
      const Image* image = is.readImage(file, recorderStats);
      defaultServer_->loadImage(image, image->x_offset, image->y_offset);
      defaultServer_->out->clear();
      delete image;
    }

    std::vector<BestEncoder> bestEncoders =  bruteForceEncoder->bestEncoders();

    unsigned long long totalInput = 0;
    unsigned long long totalOutput = 0;
    double totalTime = 0;
    for (const auto& encoder : bestEncoders) {
      totalInput += encoder.inputSize;
      totalOutput += encoder.outputSize;
      totalTime += encoder.encodingTime;
    }

    double ratio = (double)totalInput / totalOutput;
    std::cout << "Time (ms): " << totalTime << "\n"
              << "Ratio: " << ratio << "\n";

    for (const auto& b : bruteForceEncoder->bestCounter()) {
      std::string encoder = encoderClasstoString(b.first);
      int count = b.second;
      std::cout << encoder << " " << count << "\n";
    }
  }

  void BruteForce::setBruteForceEncoder(TimedBruteForceEncoder *encoder)
  {
    bruteForceEncoder = encoder;
    manager->setActiveEncoder(bruteForceEncoder);
  }

  void BruteForce::setDefaultBruteForceEncoder()
  {
    std::vector<TimedEncoder*> encoders = {
      new TimedRawEncoder(server),
      new TimedRREEncoder(server),
      new TimedHextileEncoder(server),
      new TimedTightEncoder(server),
      new TimedTightJPEGEncoder(server),
      new TimedZRLEEncoder(server),
    };

    // FIXME: don't hardcode values
    dynamic_cast<TimedTightEncoder*>(encoders[3])->setCompressLevel(2);
    dynamic_cast<TimedTightJPEGEncoder*>(encoders[4])->setQualityLevel(8);
    dynamic_cast<TimedZRLEEncoder*>(encoders[5])->setCompressLevel(6);

    bruteForceEncoder = new TimedBruteForceEncoder(server, encoders);
    manager->setActiveEncoder(bruteForceEncoder);
  }
}
