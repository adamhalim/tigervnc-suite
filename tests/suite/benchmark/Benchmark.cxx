#include "Benchmark.h"
#include "../fileUtil.h"
#include "rdr/Exception.h"
#include "rfb/EncodeManager.h"
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
    Image* image = is.readImage(file);
    // FIXME: Not sure if we should multiply by 3 or 4? 
    stats.outputSize += image->width * image->height * 3;

    stats.startClock();
    // FIXME: This will in practice load the entire file to memory as the 
    // Server writes to its InputStream for each update.
    server_->loadImage(image, image->x_offset, image->y_offset);
    stats.stopClock();
    delete image;
  }

  // FIXME: This takes the compression ration including data taken up by the
  // RFB protocol. Needs to be changed if we only care about the actual image
  // compression ratio.
  stats.inputSize = server_->out->length();
  std::vector<encoderStats> encoderStats = server_->stats();

  std::cout << "Benchmarking complete!\n"
            << "Encoders used:\n\n";
  double totalEncodingtime = 0;
  int tableWidth = 20;
  for(struct encoderStats es : encoderStats) {
    std::cout << "\t" << es.name << " encoder: (seconds)\n\t\t"
              << std::setprecision(5) << std::fixed 
              << std::setw(tableWidth) << std::setfill(' ') << std::left
              << "writeRect: " << std::right << es.writeRectEncodetime
              << "\n\t\t" << std::left  << std::setw(tableWidth)
              << "writeSolidRect: " << std::right 
              << es.writeSolidRectEncodetime << "\n\t\t"
              << std::setw(tableWidth) << std::left
              << "total: " << std::right << es.writeRectEncodetime 
                                          + es.writeSolidRectEncodetime
              << std::endl;
    totalEncodingtime += es.writeRectEncodetime + es.writeSolidRectEncodetime;
  }

  tableWidth  = 30;
  std::cout << "\nBENCHMARK SUMMARY:\n"
            << std::setw(tableWidth) << std::fixed << std::setfill(' ')
            << std::left 
            << "Encoders time:" 
            << std::right << totalEncodingtime << "\n"
            << std::setw(tableWidth) << std::left
            << "EncodeManager time:" 
            << std::right << stats.time() << "\n"
            << std::setw(tableWidth) << std::left
            << "Compression ratio (RFB):" 
            << std::right << stats.compressionRatio() << "\n"
            << std::setw(tableWidth) << std::left
            << "\% time spent encoding:"
            << std::right << totalEncodingtime / stats.time()
            << " (EncodeManager)\n";
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