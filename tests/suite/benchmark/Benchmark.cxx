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
  EncoderSettings* settings = new EncoderSettings[ENCODERS_COUNT];
  for (uint i = 0; i < ENCODERS_COUNT; i++) {
    settings[i] = encoderSettings(static_cast<EncoderClass>(i));
  }
  runBenchmark(settings, ENCODERS_COUNT);
  delete [] settings;
}

void Benchmark::runBenchmark(EncoderSettings* settings, size_t len)
{
  FrameInStream is =  FrameInStream();
  std::ifstream file(filename);
  is.parseHeader(file); // FIXME: Don't parse header twice

  // Use one Server instance per encoding
  std::map<EncoderClass, Server*> servers;
  for (uint i = 0; i < len; i++) {
    EncoderSettings setting = settings[i];
    // All encoders in the server will be of one encoder type.
    Server* s = new Server(this->width(), this->height(), setting);

    if (setting.encoderClass == encoderTightJPEG) {
      const int encodings[2] = { setting.rfbEncoding,
                                 setting.quality + 
                                 rfb::pseudoEncodingQualityLevel0 };
      s->setEncodings(2, encodings);
    } else {
      s->setEncodings(1, &setting.rfbEncoding);
    }
    servers[setting.encoderClass] = s;
      
  }

  std::cout << "Starting benchmark using \"" << filename << "\"\n";
  while(file.peek() != EOF) {
    Image* image = is.readImage(file);

    // For each encoding we want to test, we load an image and loop through 
    // all servers
    for (uint i = 0; i < len; i++) {
      EncoderSettings setting = settings[i];
      Server* server = servers[setting.encoderClass];
      server->loadImage(image, image->x_offset, image->y_offset);
      server->out->clear();
    }
    delete image;
  }
  std::cout << "Benchmarking complete!\n";

  // Loop through each server and print the corresponding statistics
  for(auto &s : servers) {
    // FIXME: Refactor this to a separate function
    std::string encoderRequested = encoderClasstoString(s.first);
    Server* server = s.second;
    std::map<EncoderClass, encoderStats> encoderStats = server->stats();

    if(!encoderStats.size())
      continue; // FIXME: throw/log error?

    int tableWidth = 35;
    int precision = 5;
    for(const auto& es : encoderStats) {
       struct encoderStats stats = es.second;

        // FIXME: Wrap this monstrosity in smaller functions :^)
        std::cout << "\n\t" << stats.name << " encoder: (seconds)\n\t\t"
                  << std::setprecision(precision) << std::fixed 
                  << std::setw(tableWidth) << std::setfill(' ') << std::left
                  << "writeRect: " << std::right << stats.writeRectEncodetime
                  << "\n\t\t" << std::left  << std::setw(tableWidth)
                  << "writeSolidRect: " << std::right 
                  << stats.compressionRatioSolidRects()
                  << "\n\t\t" << std::setw(tableWidth) << std::left
                  << "total: " << std::right << stats.writeRectEncodetime 
                                              + stats.writeSolidRectEncodetime
                  << "\n\t\t" << std::setfill('-')
                  << std::setw(tableWidth+precision+2)
                  << std::left << "" << std::setfill(' ')
                  << "\n\t\t" << std::setw(tableWidth) << std::left  
                  << "# rects: " << std::right << stats.nRects
                  << "\n\t\t" << std::setw(tableWidth) << std::left
                  << "# solidRects: " << std::right << stats.nSolidRects 
                  << "\n\t\t" << std::setfill('-') 
                  << std::setw(tableWidth+precision+2)
                  << std::left << "" << std::setfill(' ')
                  << "\n\t\t" << std::setw(tableWidth) << std::left
                  << "MPx/s (rects):" << std::right 
                  << stats.megaPixelsPerSecondRects() << "\n\t\t"
                  << std::setw(tableWidth) << std::left
                  << "MPx/s (solidRects:)" << std::right 
                  << stats.megaPixelsPerSecondSolidRects() << "\n\t\t"
                  << std::setw(tableWidth) << std::left 
                  << "MPx/s (combined)" << std::right
                  << stats.megaPixelsPerSecondCombined() << "\n\t\t"
                  << std::setfill('-') 
                  << std::setw(tableWidth+precision+2)
                  << std::left << "" << std::setfill(' ') << "\n\t\t"
                  << std::setw(tableWidth) << std::left
                  << "Compression ratio rects: "
                  << stats.compressionRatioRects()
                  << "\n\t\t" << std::setw(tableWidth) << std::left
                  << "Compression ratio solidRects: " << std::right 
                  << stats.compressionRatioSolidRects() << "\n\t\t"
                  << std::setw(tableWidth) << std::left
                  << "Compression ratio combined: " 
                  << stats.compressionRatioCombined() << "\n\t\t"
                  << std::setfill('-') << std::setw(tableWidth)
                  << std::setw(tableWidth+precision+2)
                  << std::left << "" << std::setfill(' ') << "\n\t\t"
                  << std::setw(tableWidth) << std::left
                  << "Score (time * compression ratio):" << std::right
                  << stats.score() << "\n\t\t" << std::setfill('-') 
                  << std::setw(tableWidth+precision+2)
                  << std::left << "" << std::endl;
    }
  delete server;
  }
}

EncoderSettings Benchmark::encoderSettings(EncoderClass encoderClass,
                                           PseudoEncodingLevel quality,
                                           PseudoEncodingLevel compression)
{
  EncoderSettings settings {
    .encoderClass = encoderClass,
    .rfbEncoding = rfb::encodingRaw,
    .quality = quality,
    .compression = compression,
  };

  switch(encoderClass) {
      case encoderRaw:
        settings.rfbEncoding = rfb::encodingRaw;
        break;
      case encoderRRE:
        settings.rfbEncoding = rfb::encodingRRE;
        break;
      case encoderHextile:
        settings.rfbEncoding = rfb::encodingHextile;
        break;
      case encoderTight:
        settings.rfbEncoding = rfb::encodingTight;
        break;
      case encoderTightJPEG:
        if (quality == NONE)
          settings.quality = TWO;
        settings.rfbEncoding = rfb::encodingTight;
        break;
      case encoderZRLE:
        settings.rfbEncoding = rfb::encodingZRLE;
        break;
      default:
        throw std::logic_error("EncoderClass not implemented");
  }
  return settings;
}