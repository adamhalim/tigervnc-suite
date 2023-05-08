#include "TimedTightJPEGEncoder.h"
#include "TimedEncoder.h"
#include "rfb/SConnection.h"
#include <iostream>

namespace suite {

  TimedTightJPEGEncoder::TimedTightJPEGEncoder(SConnection* conn_) 
                            : TightJPEGEncoder(conn_), 
                              TimedEncoder(encoderTightJPEG)
  {
    os = conn->getOutStream();
    is = conn->getInStream();
    conn_ = conn;
  }

  TimedTightJPEGEncoder::~TimedTightJPEGEncoder()
  {
  }

  void TimedTightJPEGEncoder::writeRect(const PixelBuffer* pb,
                                    const Palette& palette)
  {
    startWriteRectTimer(this->conn);
    TightJPEGEncoder::writeRect(pb, palette);
    stopWriteRectTimer(pb);
  }

  void TimedTightJPEGEncoder::writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour)
  {
    startWriteSolidRectTimer(this->conn);
    TightJPEGEncoder::writeSolidRect(width, height, pf, colour);
    stopWriteSolidRectTimer(width, height);
  }
}