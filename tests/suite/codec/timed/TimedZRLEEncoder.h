#ifndef __SUITE_TIMED_ZRLEENCODER_H__
#define __SUITE_TIMED_ZRLEENCODER_H__
#include "TimedEncoder.h"
#include "rfb/ZRLEEncoder.h"
#include "rfb/SConnection.h"

namespace suite {

  using namespace rfb;

  class TimedZRLEEncoder : public ZRLEEncoder, public TimedEncoder
  {
  public:
    TimedZRLEEncoder(SConnection* conn_);
    ~TimedZRLEEncoder();

    virtual void writeRect(const PixelBuffer* pb, const Palette& palette);
    virtual void writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour);
  };

}

#endif // __SUITE_TIMED_ZRLEENCODER_H__
