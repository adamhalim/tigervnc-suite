#include "TimedRREEncoder.h"
#include "TimedEncoder.h"
#include "rfb/SConnection.h"
#include "../../Manager.h"
#include <iostream>

namespace suite {

  TimedRREEncoder::TimedRREEncoder(SConnection* conn_) 
                                  : RREEncoder(conn_),
                                    TimedEncoder(encoderClassName(encoderRRE))
  {
    os = conn->getOutStream();
    is = conn->getInStream();
    conn_ = conn;
  }

  TimedRREEncoder::~TimedRREEncoder()
  {
  }

  void TimedRREEncoder::writeRect(const PixelBuffer* pb,
                                    const Palette& palette)
  {
    startWriteRectTimer(this->conn);
    RREEncoder::writeRect(pb, palette);
    stopWriteRectTimer(pb);
  }

  void TimedRREEncoder::writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour)
  {
    startWriteSolidRectTimer(this->conn);
    RREEncoder::writeSolidRect(width, height, pf, colour);
    stopWriteSolidRectTimer(width, height);
  }
}