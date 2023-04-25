#include "FrameOutStream.h"
#include <cstdio>
#include <fstream>

namespace suite {

  FrameOutStream::FrameOutStream(std::string filename, ImageDecoder* decoder)
                                : head(buffer), end(buffer + BUF_SIZE),
                                  headerWritten(false), decoder(decoder->name)
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
    lock.lock();
    if (!check(image->size))
    {
      // FIXME: Investigate if it's worth to compress the buffer before writing
      // to save storage and IO.
     head = buffer;
    }

    file << image->size << " " << image->width << " " << image->height << " "
         << image->x_offset << " " << image->y_offset<< "\n";
    file.write((char*)image->getBuffer(), image->size);
    file << "\n";
    head += image->size;
    lock.unlock();
  }

  void FrameOutStream::addUpdate(rdr::U8* data, int width, int height,
                                int x_offset, int y_offset, int size)
  {
    Image* image = new Image(width, height, data, size, x_offset, y_offset);
    ImageUpdate* update = new ImageUpdate(image);
    addUpdate(update);
  }

  bool FrameOutStream::check(size_t size)
  {
    return head + size > end;
  }

  void FrameOutStream::writeHeader(int width, int height)
  {
    if (headerWritten)
      throw std::logic_error("header already written");
    file << decoder << " " << width << " " << height << "\n";
    headerWritten = true;
  }
}