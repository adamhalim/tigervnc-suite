#include "TimedTightJPEGEncoder.h"
#include "TimedEncoder.h"
#include "rfb/SConnection.h"
#include "../../Manager.h"
#include <iostream>

namespace suite {

  TimedTightJPEGEncoder::TimedTightJPEGEncoder(SConnection* conn_) 
                            : TightJPEGEncoder(conn_), 
                              TimedEncoder(encoderClassName(encoderTightJPEG))
  {
  }

  TimedTightJPEGEncoder::~TimedTightJPEGEncoder()
  {
  }

  void TimedTightJPEGEncoder::writeRect(const PixelBuffer* pb,
                                    const Palette& palette)
  {
    startWriteRectTimer();
    TightJPEGEncoder::writeRect(pb, palette);
    stopWriteRectTimer();
  }

  void TimedTightJPEGEncoder::writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour)
  {
    startWriteSolidRectTimer();
    TightJPEGEncoder::writeSolidRect(width, height, pf, colour);
    stopWriteSolidRectTimer();
  }
}