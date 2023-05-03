#include "PPMDecoder.h"
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

namespace suite {
  PPMDecoder::PPMDecoder() : ImageDecoder(PPM)
  {
  }


  PPMDecoder::~PPMDecoder()
  {
  }

  // Leaves ownership of Image
  Image *PPMDecoder::decodeImageFromFile(std::string filename)
  {
    unsigned int width;
    unsigned int height;
    std::string version;
    int maxVal;

    std::ifstream is(filename.c_str());
    is >> version; // ignored
    is >> width;
    is >> height;
    is >> maxVal; // ignored

    std::string line;
    std::string pixels;
    // Load entire image into one string, since PPM images don't
    // necessarily have columns of same length.
    while (std::getline(is, line)) {
      pixels += line + " ";
    }
    is.close();

    std::istringstream iss(pixels);
    int bufSize = width * height * 3;
    rdr::U8* buf = new rdr::U8[bufSize];
    Image *image = new Image(width, height, buf, bufSize);
    // While there is data still in stream, 
    while ((iss >> std::ws).peek() != std::char_traits<char>::eof()) {
      Pixel pixel;
      iss >> pixel;
      *image += pixel;
    }
    measurePixelRate(width, height, 3);
    return image;
  }

  Image* PPMDecoder::decodeImageFromMemory(rdr::U8* data, int width,
                                           int height, int size,
                                           int x_offest, int y_offset)
  {
    throw std::logic_error("function not implemented");
  }

  void PPMDecoder::encodeImageTofile(Image *image, std::string filename)
  {
    encodeImageTofile(image->getBuffer(), image->width, image->height, filename);
  }

  void PPMDecoder::encodeImageTofile(const rdr::U8* data,
                                     int width, int height,
                                     std::string filename)
  {
    std::ofstream file;
    file.open(filename.c_str());

    if (!file.is_open()) {
      throw std::ofstream::failure("error creating file \"" + filename + "\"");
    }

    // Header/metadata
    const int MAXVAL = 255;
    file << "P3" << "\n" << width << " " << height << "\n" 
      << MAXVAL << "\n";

    std::ostringstream oss;
    for (int i = 0; i < width * height * 4; i+=4) {
      rdr::U8 r = data[i];
      rdr::U8 g = data[i+1];
      rdr::U8 b = data[i+2];
      oss << (int)r << " " << (int)g << " " << (int)b << " ";
    }
    file << oss.str();
    file.close();
    measurePixelRate(width, height, 3);
  }

  Image* PPMDecoder::encodeImageToMemory(const rdr::U8 *data, int width,
                                        int height, int offset_x, int offset_y)
  {
    const std::string MAXVAL = "255";
    std::string header = "P3\n" + std::to_string(width) + " " 
      + std::to_string(height) + "\n" + MAXVAL + "\n";
    rdr::U8* buf = new rdr::U8[(width * height * 3) + header.length()];

    for (int i = 0; i < width * height * 4; i+=4) {
      buf[i - (i/4)] = data[i];
      buf[i +1 - (i/4)] = data[i + 1];
      buf[i + 2 - (i/4)] = data[i+ 2];
    }

    Image* image = new Image(width, height, buf,
                             width * height * 3,
                             offset_x, offset_y);
    measurePixelRate(width, height, 3);
    return image;
  }
}

