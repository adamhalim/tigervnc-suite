#include "TimedHextileEncoder.h"
#include "TimedEncoder.h"
#include "rfb/SConnection.h"
#include "../../Manager.h"
#include <iostream>

namespace suite {

  TimedHextileEncoder::TimedHextileEncoder(SConnection* conn_) 
                              : HextileEncoder(conn_), 
                                TimedEncoder(encoderClassName(encoderHextile))
  {
  }

  TimedHextileEncoder::~TimedHextileEncoder()
  {
  }

  void TimedHextileEncoder::writeRect(const PixelBuffer* pb,
                                    const Palette& palette)
  {
    startWriteRectTimer();
    HextileEncoder::writeRect(pb, palette);
    stopWriteRectTimer();
  }

  void TimedHextileEncoder::writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour)
  {
    startWriteSolidRectTimer();
    HextileEncoder::writeSolidRect(width, height, pf, colour);
    stopWriteSolidRectTimer();
  }
}