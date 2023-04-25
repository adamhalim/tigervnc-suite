#include "Benchmark.h"
#include "../fileUtil.h"
#include "rdr/Exception.h"
#include <chrono>
using namespace suite;

Benchmark::Benchmark(std::string dirName, ImageDecoder* decoder) 
                    : dirName(dirName), decoder(decoder)
{
  this->files = readDirectory(dirName);
  if (dirName.back() != '/')  {
    dirName += '/';
  }

  this->offsets = loadOffsets(dirName + OFFSETS_FILENAME);
}

Benchmark:: Benchmark(std::string dirName, ImageDecoder* decoder, 
                      std::vector<std::pair<double, double> > offsets) 
                      : dirName(dirName), decoder(decoder), offsets(offsets)
{
    this->files = readDirectory(dirName);
}

Benchmark::~Benchmark()
{
  delete decoder;
}

void Benchmark::runBenchmark(Server* server)
{
  if (offsets.empty()) {
    for(unsigned int i = 0; i < files.size(); i++) {
      offsets.push_back(std::make_pair(0,0));
    }
  } else if (offsets.size() != files.size()) {
    throw rdr::Exception("offsets and files mismatch");
  }

  
  for (unsigned int i = 0; i < files.size(); i++) {
    Image* image = decoder->decodeImageFromFile(files.at(i));

    stats.startClock();
    server->loadImage(image, offsets.at(i).first, offsets.at(i).second);
    stats.stopClock();
    delete image;
  }
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