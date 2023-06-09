#include "JPEGDecoder.h"
#include "ImageDecoder.h"
#include "rfb/JpegDecompressor.h"
#include "rfb/ServerParams.h"
#include "rfb/TightDecoder.h"
#include "rfb/TightJPEGEncoder.h"
#include <fstream>
#include <fstream>
#include <cstring>

namespace suite {
  rfb::PixelFormat pf(32, 24, false, true,
                      255, 255, 255, 0, 8, 16);

  JPEGDecoder::JPEGDecoder(int quality, int subsampling) 
  : ImageDecoder(JPEG), quality(quality), subsampling(subsampling)
  {
  }

  JPEGDecoder::~JPEGDecoder()
  {
  }


  Image* JPEGDecoder::decodeImageFromFile(std::string filename)
  {
    throw std::logic_error("method not implemented");
  }

  Image* JPEGDecoder::decodeImageFromMemory(rdr::U8 *data, int width,
                                            int height, int size,
                                            int x_offset, int y_offset)
  {
    rfb::Rect rect;
    rdr::U8* buffer;
    Image* image;
    rfb::JpegDecompressor jd;

    rect.tl.x = x_offset;
    rect.tl.y = y_offset;
    rect.br.x = x_offset + width;
    rect.br.y = y_offset + height;

    buffer = new rdr::U8[width * height * 4];

    jd.decompress(data, size, buffer, width, rect, pf);

    image = new Image(width, height, buffer, size, x_offset, y_offset);
    measurePixelRate(width, height, 4);
    return image;
  }

  void JPEGDecoder::encodeImageTofile(const rdr::U8 *data, int width,
                                      int height, std::string filename)
  {
    rfb::Rect rect;
    std::ofstream of;

    rect.tl.x = 0;
    rect.tl.y = 0;
    rect.br.x = width;
    rect.br.y = height;

    jc.clear();
    jc.compress(data, width, rect, pf, quality, subsampling);

    of.open(filename);
    of.write((char*)jc.data(), jc.length());
    of.close();
  }

  Image* JPEGDecoder::encodeImageToMemory(const rdr::U8 *data, int width,
                                          int height, int x_offset,
                                          int y_offset)
  {
    rfb::Rect rect;
    rdr::U8 *bufferCopy;
    Image* image;

    rect.tl.x = x_offset;
    rect.tl.y = y_offset;
    rect.br.x = x_offset + width;
    rect.br.y = y_offset + height;

    jc.clear();
    jc.compress(data, width, rect, pf, quality, subsampling);

    bufferCopy = new rdr::U8[jc.length()];
    memcpy(bufferCopy, jc.data(), jc.length());

    image = new Image(width, height, bufferCopy, jc.length(), 
                             x_offset, y_offset);
    measurePixelRate(width, height, 4);
    return image;
  }
}
