#ifndef __SUITE_IMAGE_H__
#define __SUITE_IMAGE_H__

#include "rdr/types.h"
#include "../recorder/util.h"
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
          int size, int offset_x = 0, int offset_y = 0,
          uint frameTime = 0, IntersectionStats stats = IntersectionStats{});
    virtual ~Image();
    virtual Image& operator+=(Pixel const &pixel);
    virtual rdr::U8* getBuffer() const;
    const int width, height, x_offset, y_offset;
    IntersectionStats stats;
    uint size, frameTime;
    void setBuffer(rdr::U8 *buffer, int size);
  private:
    rdr::U8* buffer;
    unsigned int pixelCount;
  };
}
#endif // __SUITE_IMAGE_H__
