#include "Benchmark.h"
#include "../fileUtil.h"
#include "rdr/Exception.h"
#include "rfb/Exception.h"
#include <exception>
#include <fstream>
#include <chrono>
#include <ios>
#include <stdexcept>
#include <sys/stat.h>
#include <iomanip>
using namespace suite;

Benchmark::Benchmark(std::string filename) : filename(filename)
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

  std::cout << "Starting benchmark using \"" << filename << "\"\n";
  while(file.peek() != EOF) {
    Image* image = is.readImage(file, stats.inputSize);
    // FIXME: Not sure if we should multiply by 3 or 4? 
    stats.outputSize += image->width * image->height * 3;

    stats.startClock();
    // FIXME: This will in practice load the entire file to memory as the 
    // Server writes to its InputStream for each update.
    server_->loadImage(image, image->x_offset, image->y_offset);
    stats.stopClock();
    delete image;
  }

  std::cout << "Benchmark complete!\n"
            << std::setprecision(17)
            << "\tTotal encoding time:\t" << stats.time() << " seconds\n"
            << "\tCompression ratio:\t" << stats.compressionRatio()
            << "\n";
}


double Benchmark::getTime()
{
  return stats.time();
}

double stats::time()
{
  return encodeTime;
}
void stats::startClock()
{
  this->start = std::chrono::steady_clock::now();
}

void stats::stopClock()
{
  this->end = std::chrono::steady_clock::now();

  std::chrono::duration<double> time = end - start;
  encodeTime += time.count();
}

double stats::compressionRatio() {
  return (double)inputSize / outputSize;
}