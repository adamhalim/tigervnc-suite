#include "ImageUpdate.h"

namespace suite {
  ImageUpdate::ImageUpdate(Image* image) : image(image)
  {
  }
  ImageUpdate::~ImageUpdate()
  {
    delete image;
  }
}