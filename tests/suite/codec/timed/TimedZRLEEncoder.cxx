#include "TimedZRLEEncoder.h"
#include "TimedEncoder.h"
#include <rfb/SConnection.h>
#include <iostream>

namespace suite {

  TimedZRLEEncoder::TimedZRLEEncoder(SConnection* conn_, Manager* manager) 
    : ZRLEEncoder(conn_), TimedEncoder(encoderZRLE, manager)
  {
  }

  TimedZRLEEncoder::~TimedZRLEEncoder()
  {
  }

  void TimedZRLEEncoder::writeRect(const PixelBuffer* pb,
                                   const Palette& palette)
  {
    startWriteRectTimer(this->conn);
    ZRLEEncoder::writeRect(pb, palette);
    stopWriteRectTimer(pb);
  }

  void TimedZRLEEncoder::writeSolidRect(int width, int height,
                                        const PixelFormat& pf,
                                        const rdr::U8* colour)
  {
    startWriteSolidRectTimer(this->conn);
    ZRLEEncoder::writeSolidRect(width, height, pf, colour);
    stopWriteSolidRectTimer(width, height);
  }
}