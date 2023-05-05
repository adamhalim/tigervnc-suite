#include "Benchmark.h"
#include "../fileUtil.h"
#include "rdr/Exception.h"
#include "rfb/EncodeManager.h"
#include "rfb/Exception.h"
#include "rfb/encodings.h"
#include <exception>
#include <fstream>
#include <chrono>
#include <ios>
#include <stdexcept>
#include <sys/stat.h>
#include <iomanip>

using namespace suite;

Benchmark::Benchmark(std::string filename, const rdr::S32* encodings,
                                           const size_t len) 
                                           : filename(filename),
                                             encodings_(encodings),
                                             encodingsLength_(len)
{
  struct stat buf;
  if (stat (filename.c_str(), &buf) != 0)
    throw std::ios_base::failure("file does not exist");

  FrameInStream is =  FrameInStream();
  std::ifstream file(filename);
  file = std::ifstream(filename);

  int width;
  int height;
  std::tie(width, height) = is.parseHeader(file);

  this->width_ = width;
  this->height_ = height;
  try {
    // FIXME: this Server is unused in this class, but we still want to enable
    // debugging using the Client somehow. Maybe we always want to have a
    // default server & encoding that can be used for debugging the output?
    this->server_ = new Server(width, height);
  } catch (rfb::Exception &e) {
    throw e;
  }
}

Benchmark::~Benchmark()
{
  delete server_;
}

void Benchmark::runBenchmark()
{
  FrameInStream is =  FrameInStream();
  std::ifstream file(filename);
  is.parseHeader(file); // FIXME: Don't parse header twice

  // Use one Server instance per encoding
  std::map<int, Server*> servers;
  for(unsigned int i = 0; i < encodingsLength_; i++) {
    int encoder = encodings_[i];
    servers[encoder] = new Server(this->width(), this->height());
  }

  std::cout << "Starting benchmark using \"" << filename << "\"\n";
  while(file.peek() != EOF) {
    Image* image = is.readImage(file);

    // For each image we load, we loop each server and pass our 
    // preferred encoding.
    for(unsigned int i = 0; i < encodingsLength_; i++) {
      int encoder = encodings_[i];
      Server* server = servers[encoder];

      if(encoder < 0) // FIXME: Support quality & compress pseudoencodings
        continue;

      server->setEncodings(1, &encoder);
      server->loadImage(image, image->x_offset, image->y_offset);
      server->out->clear();
    }
    delete image;
  }
  std::cout << "Benchmarking complete!\n";

  // Loop through each server and print the corresponding statistics
  for(auto &s : servers) {
    // FIXME: Refactor this to a separate function
    std::string encoderRequested = encodingToString(s.first);
    Server* server = s.second;
    std::map<const int, encoderStats> encoderStats = server->stats();

    if(!encoderStats.size())
      continue; // FIXME: throw/log error?

    std::cout << "Encoder set: " << encoderRequested << ".\n"
              << "\tEncoder(s) used:\n";
    double totalEncodingtime = 0;
    int tableWidth = 20;
    for(const auto& es : encoderStats) {
       struct encoderStats stats = es.second;

        std::cout << "\t\t" << stats.name << " encoder: (seconds)\n\t\t\t"
                  << std::setprecision(5) << std::fixed 
                  << std::setw(tableWidth) << std::setfill(' ') << std::left
                  << "writeRect: " << std::right << stats.writeRectEncodetime
                  << "\n\t\t\t" << std::left  << std::setw(tableWidth)
                  << "writeSolidRect: " << std::right 
                  << (stats.writeSolidRectEncodetime != 0 ? 
                        std::to_string(stats.writeSolidRectEncodetime)
                            : "-" ) // FIXME: the string is 1 char too long
                  << "\n\t\t\t"
                  << std::setw(tableWidth) << std::left
                  << "total: " << std::right << stats.writeRectEncodetime 
                                              + stats.writeSolidRectEncodetime
                    << "\n\t\t\t" << std::setw(tableWidth) << std::left 
                    << "Compression ratio: " << std::right
                    <<  stats.outputSize / (double) stats.inputSize 
                    << std::endl;

      totalEncodingtime += stats.writeRectEncodetime 
                         + stats.writeSolidRectEncodetime;
    }

  if(totalEncodingtime) {} // FIXME: use this value...
  delete server;
  }
}
