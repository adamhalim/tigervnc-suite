#include "TimedRawEncoder.h"
#include "TimedEncoder.h"
#include "rfb/SConnection.h"
#include <iostream>

namespace suite {

  TimedRawEncoder::TimedRawEncoder(SConnection* conn_, Manager* manager) 
                                 : RawEncoder(conn_), 
                                   TimedEncoder(encoderRaw, manager)
  {
  }

  TimedRawEncoder::~TimedRawEncoder()
  {
  }

  void TimedRawEncoder::writeRect(const PixelBuffer* pb,
                                  const Palette& palette)
  {
    startWriteRectTimer(this->conn);
    RawEncoder::writeRect(pb, palette);
    stopWriteRectTimer(pb);
  }

  void TimedRawEncoder::writeSolidRect(int width, int height,
                                       const PixelFormat& pf,
                                       const rdr::U8* colour)
  {
    startWriteSolidRectTimer(this->conn);
    RawEncoder::writeSolidRect(width, height, pf, colour);
    stopWriteSolidRectTimer(width, height);
  }
}