#include "Image.h"
#include "rdr/Exception.h"
#include <cassert>
#include <cstring>
#include <sstream>

namespace suite {

  Image::Image(int width, int height, int x_offset, int y_offset) 
              : width(width), height(height), x_offset(x_offset),
                y_offset(y_offset), size(width * height * 4)              
  {
    if (width <= 0 || height <= 0)
      throw rdr::Exception("width and height need to be > 0");
      
    buffer = new rdr::U8[size];
    pixelCount = 0;
  }

  Image::Image(int width, int height, rdr::U8* buffer, int size, int x_offset,
              int y_offset)
              : width(width), height(height), x_offset(x_offset),
                y_offset(y_offset), size(size), buffer(buffer)
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

  rdr::U8* Image::getBuffer()
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
