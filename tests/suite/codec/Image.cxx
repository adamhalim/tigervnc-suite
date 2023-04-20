#include "Image.h"
#include "rdr/Exception.h"
#include <cassert>
#include <cstring>
#include <sstream>

Image::Image(int width, int height) : width(width), height(height), bufferSize(width * height * 4)
{
  if (width <= 0 || height <= 0)
    throw rdr::Exception("width and height need to be > 0");
    
  buffer = new rdr::U8[bufferSize];
  pixelCount = 0;
}

Image::~Image()
{
  delete [] buffer;
}

Image &Image::operator+=(Pixel const &pixel)
{
  assert(pixelCount * 4 <= bufferSize);   
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

void Image::setBuffer(rdr::U8 *buffer)
{
  this->buffer = buffer;
}