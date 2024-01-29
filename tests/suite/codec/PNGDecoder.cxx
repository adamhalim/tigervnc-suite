#include "PNGDecoder.h"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

#include "lib/fpng.h"

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

namespace suite {

  PNGDecoder::PNGDecoder() : ImageDecoder(PNG)
  {
    fpng::fpng_init();
  }

  PNGDecoder::~PNGDecoder()
  {
  }

  Image* PNGDecoder::decodeImageFromFile(std::string filename)
  {
    // uses stb_image for decoding
    int width;
    int height;
    int channels;

    rdr::U8* data = (rdr::U8*) stbi_load(filename.c_str(), &width, &height,
                                        &channels, 0);
    int bufSize = width * height * channels;

    Image* image = new Image(width, height, data, bufSize);
    measurePixelRate(width, height, channels);
    return image;
  }

 Image* PNGDecoder::decodeImageFromMemory(rdr::U8* data, int width, int height,
                                          int size, int x_offset, int y_offset)
  {

    std::vector<uint8_t> out;
    uint32_t w;
    uint32_t h;
    uint32_t channels;
    int ret = fpng::fpng_decode_memory(data, size, out, w, h, channels, 4);
    if (ret)
      throw std::ios_base::failure("fpng: failed to decode image\n");

    rdr::U8* vectoryCopy = new rdr::U8[width * height * 4];
    std::copy(out.begin(), out.end(), vectoryCopy);

    Image* image = new Image(width, height, vectoryCopy, out.size(),
                             x_offset, y_offset);
    measurePixelRate(width, height, channels);
    return image;
  }

  void PNGDecoder::encodeImageTofile(const rdr::U8 *data, int width,
                                     int height, std::string filename)
  {
    // We use fpng to encode images as it's faster than stb_image.
    // FIXME: images encoded by fpng must be decoded using fpng.
    // Won't work using stb_image for example
    rdr::U8* paddedBuf = addAlphaPadding(data, width, height);
    fpng::fpng_encode_image_to_file(filename.c_str(),
                                    paddedBuf, width, height, 4);
    delete paddedBuf;
    measurePixelRate(width, height, 4);
  }

  Image* PNGDecoder::encodeImageToMemory(const rdr::U8 *data, int width,
                                        int height, int x_offset, int y_offset)
  {
    rdr::U8* paddedBuf = addAlphaPadding(data, width, height);
    std::vector<rdr::U8> out;
    int ret = fpng::fpng_encode_image_to_memory(paddedBuf, width, height,
                                                4, out);
    delete paddedBuf;
    if (!ret)
      throw std::ios_base::failure("fpng: failed to encode image\n");

    rdr::U8* vectorCopy = new rdr::U8[out.size()];
    std::copy(out.begin(), out.end(), vectorCopy);

    Image* image = new Image(width, height, vectorCopy, out.size(),
                             x_offset, y_offset);
    measurePixelRate(width, height, 4);
    return image;
  }

  rdr::U8* PNGDecoder::addAlphaPadding(const rdr::U8* data,
                                       int width, int height)
  {
    // When copying data from the framebuffer, the alpha channel is set to 0
    // which causes issues when encoding PNG. We don't want anything to be
    // transparent, so we set all alpha bits to 255.
    int bufSize = width * height * 4;
    rdr::U8* paddedBuf = new rdr::U8[bufSize];

    for (int i = 0; i < bufSize; i += 4) {
      paddedBuf[i] = data[i];
      paddedBuf[i + 1] = data[i+1];
      paddedBuf[i + 2] = data[i+2];
      paddedBuf[i + 3] = 255;   // no transparency
    }
    return paddedBuf;
  }
}
