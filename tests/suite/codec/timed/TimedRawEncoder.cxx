#include "TimedRawEncoder.h"
#include "TimedEncoder.h"
#include "rfb/SConnection.h"
#include "../../Manager.h"
#include <iostream>

namespace suite {

  TimedRawEncoder::TimedRawEncoder(SConnection* conn_) 
                                 : RawEncoder(conn_),
                                   TimedEncoder(encoderClassName(encoderRaw))
  {
  }

  TimedRawEncoder::~TimedRawEncoder()
  {
  }

  void TimedRawEncoder::writeRect(const PixelBuffer* pb,
                                    const Palette& palette)
  {
    startWriteRectTimer();
    RawEncoder::writeRect(pb, palette);
    stopWriteRectTimer();
  }

  void TimedRawEncoder::writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour)
  {
    startWriteSolidRectTimer();
    RawEncoder::writeSolidRect(width, height, pf, colour);
    stopWriteSolidRectTimer();
  }
}