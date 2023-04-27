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
  rdr::U8* data = (rdr::U8*)qoi_read(filename.c_str(), &desc, 4);

  if (!data)
    throw std::ios_base::failure("qoi: error decoding image");

  int bufSize = desc.width * desc.height * desc.channels;
  Image* image = new Image(desc.width, desc.height, data, bufSize);
  measureFPS();
  return image;
 }

Image* QOIDecoder::decodeImageFromMemory(rdr::U8* data, int width, int height,
                                          int size, int x_offset, int y_offset)
{
  qoi_desc desc;
  rdr::U8* decodedImage = (rdr::U8*)qoi_decode(data, size, &desc, 4);
  if (decodedImage == NULL)
    throw std::ios_base::failure("qoi: error decoding image");

  int bufSize = desc.width * desc.height * desc.channels;
  Image* image = new Image(width, height, decodedImage, bufSize,
                            x_offset, y_offset);
  measureFPS();
  return image;
}
 void QOIDecoder::encodeImageTofile(const rdr::U8 *data, int width, int height,
                                    std::string filename)
 {
  qoi_desc desc{
    .width = (unsigned int)width,
    .height = (unsigned int)height,
    .channels = 4,
    .colorspace = QOI_SRGB
  };

  int size = qoi_write(filename.c_str(), data, &desc);

  if (!size)
    throw std::ios_base::failure("error encoding image");

  measureFPS();
 }

 Image* QOIDecoder::encodeImageToMemory(const rdr::U8 *data, int width,
                                        int height, int x_offset, int y_offset)
 {
  qoi_desc desc{
    .width = (unsigned int)width,
    .height = (unsigned int)height,
    .channels = 4,
    .colorspace = QOI_SRGB
  };

  int size;
  rdr::U8* encodedData = (rdr::U8*)qoi_encode(data, &desc,  &size);
  if (!encodedData)
    throw std::ios_base::failure("error encoding image");
  Image* image = new Image(desc.width, desc.height, encodedData,
                           size, x_offset, y_offset);
  measureFPS();
  return image;
 }
}
