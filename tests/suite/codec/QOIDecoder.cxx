#include "QOIDecoder.h"
#include "lib/fpng.h"

#define QOI_IMPLEMENTATION
#include "lib/qoi.h"

namespace suite {

 QOIDecoder::QOIDecoder() : ImageDecoder(QOI)
 {
 }

 QOIDecoder::~QOIDecoder()
 {
 }

 Image* QOIDecoder::decodeImageFromFile(std::string filename)
 {
  qoi_desc desc;
  uint8_t* data = (uint8_t*) qoi_read(filename.c_str(), &desc, 4);

  if (!data)
    throw std::ios_base::failure("qoi: error decoding image");

  int bufSize = desc.width * desc.height * desc.channels;
  Image* image = new Image(desc.width, desc.height, data, bufSize);
  measurePixelRate(desc.width, desc.height, desc.channels);
  return image;
 }

Image* QOIDecoder::decodeImageFromMemory(uint8_t* data, int width, int height,
                                         int size, int x_offset, int y_offset)
{
  qoi_desc desc;
  uint8_t* decodedImage = (uint8_t*) qoi_decode(data, size, &desc, 4);
  if (decodedImage == NULL)
    throw std::ios_base::failure("qoi: error decoding image");

  int bufSize = desc.width * desc.height * desc.channels;
  Image* image = new Image(width, height, decodedImage, bufSize,
                           x_offset, y_offset);
  measurePixelRate(width, height, desc.channels);
  return image;
}
 void QOIDecoder::encodeImageTofile(const uint8_t *data, int width, int height,
                                    std::string filename)
 {
  qoi_desc desc {
    .width = (unsigned int) width,
    .height = (unsigned int) height,
    .channels = 4,
    .colorspace = QOI_SRGB
  };

  int size = qoi_write(filename.c_str(), data, &desc);

  if (!size)
    throw std::ios_base::failure("error encoding image");

  measurePixelRate(width, height, desc.channels);
 }

 Image* QOIDecoder::encodeImageToMemory(const uint8_t *data, int width,
                                        int height, int x_offset, int y_offset)
 {
  qoi_desc desc {
    .width = (unsigned int)width,
    .height = (unsigned int)height,
    .channels = 4,
    .colorspace = QOI_SRGB
  };

  int size;
  uint8_t* encodedData = (uint8_t*) qoi_encode(data, &desc,  &size);
  if (!encodedData)
    throw std::ios_base::failure("error encoding image");
  Image* image = new Image(desc.width, desc.height, encodedData,
                           size, x_offset, y_offset);
  measurePixelRate(width, height, desc.channels);
  return image;
 }
}
