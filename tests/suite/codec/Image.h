#ifndef __SUITE_IMAGE_H__
#define __SUITE_IMAGE_H__

#include "../recorder/util.h"
#include <stdint.h>
#include <istream>

namespace suite {

  struct Pixel
  {
    uint8_t r, g, b;
    friend std::istringstream& operator>>(std::istringstream &in,
                                          Pixel& pixel);
  };

  class Image
  {
  public:
    Image(int width, int height, uint8_t* buffer,
          int size, int offset_x = 0, int offset_y = 0,
          uint frameTime = 0, ImageUpdateStats stats = ImageUpdateStats{});
    virtual ~Image();
    virtual Image& operator+=(Pixel const &pixel);
    virtual uint8_t* getBuffer() const;
    const int width, height, x_offset, y_offset;
    ImageUpdateStats stats;
    uint size, frameTime;
    void setBuffer(uint8_t *buffer, int size);
  private:
    uint8_t* buffer;
    unsigned int pixelCount;
  };
}

#endif // __SUITE_IMAGE_H__
