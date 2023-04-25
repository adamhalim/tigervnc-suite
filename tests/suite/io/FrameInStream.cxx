#include "FrameInStream.h"
#include "rdr/types.h"
#include "../codec/lib/fpng.h"
#include "../codec/lib/qoi.h"
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

namespace suite {

  FrameInStream::FrameInStream(ImageDecoder* decoder) : decoder(decoder)
  {
  }

  FrameInStream::~FrameInStream()
  {
  }

  Image* FrameInStream::readImage(std::istream& is)
  {
    int size, width, height, x_offset, y_offset;

    if (is.peek() == EOF) 
      throw std::ios_base::failure("error reading from stream");

    is >> size >> width >> height >> x_offset >> y_offset;
    is.ignore();

    rdr::U8* data = new rdr::U8[size];
    is.read((char*)data, size);

    Image* image = decoder->decodeImageFromMemory(data, width, height, size, x_offset, y_offset);
    delete [] data;

    return image;
  }
}