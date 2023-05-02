#ifndef __SUITE_IMAGE_UPDATE_H__
#define __SUITE_IMAGE_UPDATE_H__

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

#endif // __SUITE_IMAGE_UPDATE_H__
