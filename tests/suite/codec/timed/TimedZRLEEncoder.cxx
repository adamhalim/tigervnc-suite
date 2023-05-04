#include "TimedZRLEEncoder.h"
#include "TimedEncoder.h"
#include "rfb/SConnection.h"
#include "../../Manager.h"
#include <iostream>

namespace suite {

  TimedZRLEEncoder::TimedZRLEEncoder(SConnection* conn_) 
                                  : ZRLEEncoder(conn_),
                                    TimedEncoder(encoderClassName(encoderZRLE))
  {
  }

  TimedZRLEEncoder::~TimedZRLEEncoder()
  {
  }

  void TimedZRLEEncoder::writeRect(const PixelBuffer* pb,
                                    const Palette& palette)
  {
    startWriteRectTimer();
    ZRLEEncoder::writeRect(pb, palette);
    stopWriteRectTimer();
  }

  void TimedZRLEEncoder::writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour)
  {
    startWriteSolidRectTimer();
    ZRLEEncoder::writeSolidRect(width, height, pf, colour);
    stopWriteSolidRectTimer();
  }
}