#include "FrameInStream.h"
#include "rdr/types.h"
#include "../codec/lib/fpng.h"
#include "../codec/lib/qoi.h"
#include "../codec/decoderFactory.h"
#include <cstdio>
#include <fstream>
#include <ios>
#include <iostream>
#include <vector>

namespace suite {

  FrameInStream::FrameInStream() : headerParsed(false)
  {
  }

  FrameInStream::~FrameInStream()
  {
    delete decoder;
  }

  Image* FrameInStream::readImage(std::istream& is)
  {
    if (!headerParsed)
      throw std::logic_error("need to parse header before reading image");

    int size, width, height, x_offset, y_offset, frameTime;

    if (is.peek() == EOF) 
      throw std::ios_base::failure("error reading from stream");

    is >> size >> width >> height >> x_offset >> y_offset >> frameTime;
    is.ignore();

    rdr::U8* data = new rdr::U8[size];
    is.read((char*)data, size);

    Image* image = decoder->decodeImageFromMemory(data, width, height, size,
                                                  x_offset, y_offset);
    delete [] data;

    return image;
  }

  std::pair<int, int> FrameInStream::parseHeader(std::istream& is)
  {
    if (headerParsed)
      throw std::logic_error("header already parsed");
    
    std::string decoder;
    int width;
    int height;
    is >> decoder >> width >> height;
    
    this->decoder = constructDecoder(decoder);
    headerParsed = true;
    return std::make_pair(width, height);
  }
}