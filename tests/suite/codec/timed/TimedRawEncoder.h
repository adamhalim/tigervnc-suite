#ifndef __SUITE_TIMED_RAWENCODER_H__
#define __SUITE_TIMED_RAWENCODER_H__
#include "TimedEncoder.h"
#include <rfb/RawEncoder.h>
#include <rfb/SConnection.h>

namespace suite {
  using namespace rfb;

  class TimedRawEncoder : public RawEncoder, public TimedEncoder
  {
  public:
    TimedRawEncoder(SConnection* conn_);
    ~TimedRawEncoder();

    virtual void writeRect(const PixelBuffer* pb, const Palette& palette);
    virtual void writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour);
  };
}
#endif // __SUITE_TIMED_RAWENCODER_H__
