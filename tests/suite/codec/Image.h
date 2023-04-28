#ifndef __SUITE_IMAGE_H__
#define __SUITE_IMAGE_H__

#include "rdr/types.h"
#include <istream>

namespace suite {

  struct Pixel
  {
    rdr::U8 r, g, b;
    friend std::istringstream& operator>>(std::istringstream &in,
                                          Pixel& pixel);
  };

  class Image
  {
  public:
    Image(int width, int height, rdr::U8* buffer,
          int size, int offset_x = 0, int offset_y = 0);
    virtual ~Image();
    virtual Image& operator+=(Pixel const &pixel);
    virtual rdr::U8* getBuffer();
    const int width, height, x_offset, y_offset;
    unsigned int size;
    void setBuffer(rdr::U8 *buffer, int size);
  private:
    rdr::U8* buffer;
    unsigned int pixelCount;
  };
}
#endif
