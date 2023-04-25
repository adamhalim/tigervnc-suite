#ifndef __IMAGE_DECODER_H__
#define __IMAGE_DECODER_H__

#include "Image.h"
#include <chrono>
#include <iostream>

namespace suite {

  class ImageDecoder
  {
  public:
    ImageDecoder() {
      #if _DEBUG
        start = std::chrono::system_clock::now();
        frameCount = 0;
      #endif
    };
    virtual ~ImageDecoder() {};

    virtual Image *decodeImageFromFile(std::string filename) = 0;
    virtual Image* decodeImageFromMemory(rdr::U8* data, int width, int height,
                                          int size, int x_offest = 0, int y_offset = 0) = 0;
    virtual void encodeImageTofile(const rdr::U8* data, int width, int height, std::string filename) = 0;
    virtual Image* encodeImageToMemory(const rdr::U8* data, int width,
                                      int height, int offset_x = 0,
                                      int offset_y  = 0) = 0;
  protected:
    // Measures encoding/decoding performance.
    void measureFPS()
    {
      // FIXME: This method has to be called explicitly by derived classes.
      // Is there a way to do this automatically?
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
  private:
    int frameCount;
    std::chrono::system_clock::time_point start;

  };
}
#endif
