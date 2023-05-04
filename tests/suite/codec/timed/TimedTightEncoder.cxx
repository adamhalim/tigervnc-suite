#include "TimedTightEncoder.h"
#include "TimedEncoder.h"
#include "rfb/SConnection.h"
#include "../../Manager.h"
#include <iostream>

namespace suite {

  TimedTightEncoder::TimedTightEncoder(SConnection* conn_) 
                                : TightEncoder(conn_),
                                  TimedEncoder(encoderClassName(encoderTight))
  {
  }

  TimedTightEncoder::~TimedTightEncoder()
  {
  }

  void TimedTightEncoder::writeRect(const PixelBuffer* pb,
                                    const Palette& palette)
  {
    startWriteRectTimer();
    TightEncoder::writeRect(pb, palette);
    stopWriteRectTimer();
  }

  void TimedTightEncoder::writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour)
  {
    startWriteSolidRectTimer();
    TightEncoder::writeSolidRect(width, height, pf, colour);
    stopWriteSolidRectTimer();
  }
}