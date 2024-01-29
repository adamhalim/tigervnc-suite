#include "Image.h"
#include <rdr/Exception.h>
#include <cassert>
#include <cstring>
#include <sstream>

namespace suite {

  Image::Image(int width, int height, rdr::U8* buffer,
               int size, int x_offset, int y_offset, uint frameTime,
               ImageUpdateStats stats)
    : width(width), height(height), x_offset(x_offset), y_offset(y_offset),
      stats(stats), size(size), frameTime(frameTime), buffer(buffer)
  {
  }

  Image::~Image()
  {
    delete [] buffer;
  }

  Image &Image::operator+=(Pixel const &pixel)
  {
    assert(pixelCount * 4 <= size);
    memcpy(&buffer[pixelCount * 4], &pixel, 4);
    pixelCount++;
    return *this;
  }

  rdr::U8* Image::getBuffer() const
  {
    return buffer;
  }

  std::istringstream& operator>>(std::istringstream &in, Pixel& pixel)
  {
    int r, g, b;
    in >> r;
    in >> g;
    in >> b;

    pixel.r = r;
    pixel.g = g;
    pixel.b = b;
    return in;
  }

  void Image::setBuffer(rdr::U8 *buffer, int size)
  {
    delete [] this->buffer;
    this->buffer = buffer;
    this->size = size;
  }
}
