#include "ImageUpdate.h"

namespace suite {
  ImageUpdate::ImageUpdate(Image* image,
                           IntersectionStats stats) 
                         : image(image), stats(stats)
  {
  }

  ImageUpdate::~ImageUpdate()
  {
    delete image;
  }
}