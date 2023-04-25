#ifndef __BENCHMARK_FILE_H__
#define __BENCHMARK_FILE_H__

#include "../codec/Image.h"

namespace suite {

  // Represents a framebuffer update. Takes ownership of the image.
  // FIXME: Add time data so that updates can be played in "realtime".
  struct ImageUpdate 
  {
    ImageUpdate(Image* image);
    ~ImageUpdate();
    Image* image;
  };
}

#endif