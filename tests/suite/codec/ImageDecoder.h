#ifndef __SUITE_IMAGE_DECODER_H__
#define __SUITE_IMAGE_DECODER_H__

#include "Image.h"
#include <chrono>
#include <iostream>
#include <map>

namespace suite {

  enum DecoderEnum {
    PPM = 0,
    QOI,
    JPEG,
  };

  static std::map<std::string, DecoderEnum> decodersMap = {
    {"PPM", PPM},
    {"QOI", QOI},
    {"JPEG", JPEG}
  };

  static std::string decoderTypeToString(enum DecoderEnum e)
  {
    static std::string strings[] = {
      "PPM",
      "QOI",
      "JPEG",
    };
    return strings[e];
  }

  class ImageDecoder
  {
  public:
    ImageDecoder(DecoderEnum type)
      : type(type), name(decoderTypeToString(type))
    {
#ifdef _DEBUG
        start = std::chrono::system_clock::now();
        pixelCount = 0;
#endif // _DEBUG
    };
    virtual ~ImageDecoder() {};

    virtual Image *decodeImageFromFile(std::string filename) = 0;
    virtual Image* decodeImageFromMemory(uint8_t* data, int width, int height,
                                         int size, int x_offest = 0,
                                         int y_offset = 0) = 0;
    virtual void encodeImageTofile(const uint8_t* data, int width, int height,
                                   std::string filename) = 0;
    virtual Image* encodeImageToMemory(const uint8_t* data, int width,
                                       int height, int offset_x = 0,
                                       int offset_y  = 0) = 0;
    const DecoderEnum type;
    const std::string name;
  protected:
#ifdef _DEBUG
    // Measures encoding/decoding performance.
    void measurePixelRate(int width, int height, int channels)
    {
      // FIXME: This method has to be called explicitly by derived classes.
      // Is there a way to do this automatically?
      pixelCount += (width * height * channels);
      auto now = std::chrono::system_clock::now();
      std::chrono::duration<double> time = now - start;
      if (time.count() > 1) {
        printf("%.2f Mpx/s\n", pixelCount / (time.count() * 10e6));
        start = now;
        pixelCount = 0;
      }
    }
  private:
    unsigned long pixelCount;
    std::chrono::system_clock::time_point start;
#endif // _DEBUG
  };
}

#endif // __SUITE_IMAGE_DECODER_H__