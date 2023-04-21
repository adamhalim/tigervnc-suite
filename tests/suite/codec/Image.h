#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "rdr/types.h"
#include <istream>

namespace suite {

  struct Pixel
  {
    rdr::U8 r, g, b;
    friend std::istringstream& operator>>(std::istringstream &in, Pixel& pixel);
  };

  class Image
  {
  public:
    Image(int width, int height);
    virtual ~Image();
    virtual Image& operator+=(Pixel const &pixel);
    virtual rdr::U8* getBuffer();
    const int width, height;
    const unsigned int bufferSize;
    void setBuffer(rdr::U8 *buffer);
  private:
    rdr::U8* buffer;
    unsigned int pixelCount;
  };
}
#endif
