#ifndef __SUITE_IMAGE_DECODER_H__
#define __SUITE_IMAGE_DECODER_H__

#include "Image.h"
#include <chrono>
#include <iostream>
#include <map>

namespace suite {

  enum decoderEnum {
    PPM = 0,
    PNG,
    QOI
  };

  static std::map<std::string, decoderEnum> decodersMap = {
    {"PPM", PPM},
    {"PNG", PNG},
    {"QOI", QOI}
  };

  static std::string decoderTypeToString(enum decoderEnum e)
  {
    static std::string strings[] = {
      "PPM",
      "PNG",
      "QOI",
    };
    return strings[e];
  }

  class ImageDecoder
  {
  public:
    ImageDecoder(decoderEnum type) : type(type),
                                     name(decoderTypeToString(type))
    {
      #if _DEBUG
        start = std::chrono::system_clock::now();
        pixelCount = 0;
      #endif
    };
    virtual ~ImageDecoder() {};

    virtual Image *decodeImageFromFile(std::string filename) = 0;
    virtual Image* decodeImageFromMemory(rdr::U8* data, int width, int height,
                                         int size, int x_offest = 0,
                                         int y_offset = 0) = 0;
    virtual void encodeImageTofile(const rdr::U8* data, int width, int height,
                                                    std::string filename) = 0;
    virtual Image* encodeImageToMemory(const rdr::U8* data, int width,
                                      int height, int offset_x = 0,
                                      int offset_y  = 0) = 0;
    const decoderEnum type;
    const std::string name;
  protected:
    // Measures encoding/decoding performance.
    void measurePixelRate(int width, int height, int channels)
    {
      // FIXME: This method has to be called explicitly by derived classes.
      // Is there a way to do this automatically?
      #if _DEBUG
      pixelCount += (width * height * channels);
      auto now = std::chrono::system_clock::now();
      std::chrono::duration<double> time = now - start;
      if (time.count() > 1) {
        printf("%.2f Mpx/s\n", pixelCount / (time.count() * 10e6));
        start = now;
        pixelCount = 0;
      }
      #endif // _DEBUG
    }
  private:
    unsigned long pixelCount;
    std::chrono::system_clock::time_point start;
  };
}
#endif // __SUITE_IMAGE_DECODER_H__
