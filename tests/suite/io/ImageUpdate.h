#ifndef __SUITE_IMAGE_UPDATE_H__
#define __SUITE_IMAGE_UPDATE_H__

#include "../codec/Image.h"
#include "../recorder/util.h"

namespace suite {

  // Represents a framebuffer update. Takes ownership of the image.
  // FIXME: Add time data so that updates can be played in "realtime".
  struct ImageUpdate 
  {
    ImageUpdate(Image* image, IntersectionStats stats = IntersectionStats{});
    ~ImageUpdate();
    Image* image;
    IntersectionStats stats;
  };
}

#endif // __SUITE_IMAGE_UPDATE_H__
