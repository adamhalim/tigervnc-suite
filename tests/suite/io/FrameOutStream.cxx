#include "FrameOutStream.h"
#include <cstdio>
#include <fstream>

namespace suite {

  FrameOutStream::FrameOutStream(std::string filename) : head(buffer),
                                  end(buffer + BUF_SIZE)
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
    Image* image = new Image(width, height, x_offset, y_offset);
    image->setBuffer(data, size);
    ImageUpdate* update = new ImageUpdate(image);
    addUpdate(update);
  }

  bool FrameOutStream::check(size_t size)
  {
    return head + size > end;
  }
}