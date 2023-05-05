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
    os = conn->getOutStream();
    is = conn->getInStream();
    conn_ = conn;
  }

  TimedHextileEncoder::~TimedHextileEncoder()
  {
  }

  void TimedHextileEncoder::writeRect(const PixelBuffer* pb,
                                    const Palette& palette)
  {
    startWriteRectTimer(this->conn);
    HextileEncoder::writeRect(pb, palette);
    stopWriteRectTimer(pb);
  }

  void TimedHextileEncoder::writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour)
  {
    startWriteSolidRectTimer(this->conn);
    HextileEncoder::writeSolidRect(width, height, pf, colour);
    stopWriteSolidRectTimer(width, height);
  }
}