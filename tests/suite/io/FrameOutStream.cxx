#include "FrameOutStream.h"
#include <chrono>
#include <cstdio>
#include <fstream>

namespace suite {

  FrameOutStream::FrameOutStream(std::string filename, ImageDecoder* decoder)
                               : headerWritten(false), decoder(decoder->name)
  {
    file.open(filename.c_str());
    if (!file.is_open())
      throw std::ofstream::failure("error creating file");
  }

  FrameOutStream::~FrameOutStream()
  {
  }

  void FrameOutStream::addUpdate(ImageUpdate* update)
  {
    if (!headerWritten)
      throw std::logic_error("header not written");
    Image* image = update->image;
    const IntersectionStats stats = update->stats;

    // Keep track of time between frames
    auto now = std::chrono::steady_clock::now();
    auto timeSinceLastFrame = std::chrono::duration_cast
                             <std::chrono::milliseconds>
                             (now - lastFrameTime);
    lastFrameTime = now;

    lock.lock();
    file << image->size << " " << image->width << " " << image->height << " "
         << image->x_offset << " " << image->y_offset << " "
         << timeSinceLastFrame.count() << " " << stats.lostDataArea
         << " " << stats.overDimensionedArea << "\n";
    file.write((char*)image->getBuffer(), image->size);
    file << "\n";
    lock.unlock();
  }

  void FrameOutStream::addUpdate(rdr::U8* data, int width, int height,
                                int x_offset, int y_offset, int size)
  {
    Image* image = new Image(width, height, data, size, x_offset, y_offset);
    ImageUpdate* update = new ImageUpdate(image);
    addUpdate(update);
  }

  void FrameOutStream::writeHeader(int width, int height)
  {
    if (headerWritten)
      throw std::logic_error("header already written");
    file << decoder << " " << width << " " << height << "\n";
    headerWritten = true;
    lastFrameTime = std::chrono::steady_clock::now();
  }
}