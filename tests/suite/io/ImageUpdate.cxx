#include "ImageUpdate.h"

namespace suite {
  ImageUpdate::ImageUpdate(Image* image, ImageUpdateStats stats) 
    : image(image), stats(stats)
  {
  }

  ImageUpdate::~ImageUpdate()
  {
    delete image;
  }
}