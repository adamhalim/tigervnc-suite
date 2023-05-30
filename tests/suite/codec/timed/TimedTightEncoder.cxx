#include "TimedTightEncoder.h"
#include "TimedEncoder.h"
#include "rdr/InStream.h"
#include "rdr/MemOutStream.h"
#include "rdr/OutStream.h"
#include "rfb/SConnection.h"
#include <iostream>

namespace suite {

  TimedTightEncoder::TimedTightEncoder(SConnection* conn_, Manager* manager) 
                                     : TightEncoder(conn_),
                                       TimedEncoder(encoderTight, manager)
  {
  }

  TimedTightEncoder::~TimedTightEncoder()
  {
  }

  void TimedTightEncoder::writeRect(const PixelBuffer* pb,
                                    const Palette& palette)
  {
    startWriteRectTimer(this->conn);
    TightEncoder::writeRect(pb, palette);
    stopWriteRectTimer(pb);
  }

  void TimedTightEncoder::writeSolidRect(int width, int height,
                                         const PixelFormat& pf,
                                         const rdr::U8* colour)
  {
    startWriteSolidRectTimer(this->conn);
    TightEncoder::writeSolidRect(width, height, pf, colour);
    stopWriteSolidRectTimer(width, height);
  }
}