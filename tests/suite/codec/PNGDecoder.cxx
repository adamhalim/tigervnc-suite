#include "PNGDecoder.h"
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <vector>
#include "Image.h"

#include "lib/fpng.h"

#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"

namespace suite {

  PNGDecoder::PNGDecoder()
  {
    fpng::fpng_init();
    #if _DEBUG
      start = std::chrono::system_clock::now();
      frameCount = 0;
    #endif
  }

  PNGDecoder::~PNGDecoder()
  {
  }

  Image* PNGDecoder::decodeImage(std::string filename)
  {
    // uses stb_image for decoding
    int width;
    int height;
    int channels;

    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

    Image* image = new Image(width, height);
    image->setBuffer(data);
    return image;
  }

  void PNGDecoder::encodeImage(const rdr::U8 *data, int width, int height, std::string filename)
  {
    // We use fpng to encode images as it's faster than stb_image.
    // FIXME: images encoded by fpng must be decoded using fpng.
    // Won't work using stb_image for example
    rdr::U8* paddedBuf = addAlphaPadding(data, width, height);
    fpng::fpng_encode_image_to_file(filename.c_str(), paddedBuf, width, height, 4);
    delete paddedBuf;
    #if _DEBUG
      frameCount++;
      const int FRAME_SAMPLE_SIZE = 100;
      if (frameCount % FRAME_SAMPLE_SIZE == 0) {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::chrono::duration<double> time = (now - start);
        std::cout << FRAME_SAMPLE_SIZE / time.count() << " fps\n";
        start = now;
      }
      #endif
  }

  rdr::U8* PNGDecoder::addAlphaPadding(const rdr::U8* data, int width, int height)
  {
    // When copying data from the framebuffer, the alpha channel is set to 0
    // which causes issues when encoding PNG. We don't want anything to be 
    // transparent, so we set all alpha bits to 255.
    int bufSize = width * height * 4;
    rdr::U8* paddedBuf = new rdr::U8[bufSize];

    for(int i = 0; i < bufSize; i += 4) {
      paddedBuf[i] = data[i]; 
      paddedBuf[i + 1] = data[i+1]; 
      paddedBuf[i + 2] = data[i+2]; 
      paddedBuf[i + 3] = 255;   // no transparency
    }
    return paddedBuf;
  }
}
