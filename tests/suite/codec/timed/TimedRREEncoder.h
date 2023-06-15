#ifndef __SUITE_TIMED_TIGHTPEGENCODER_H__
#define __SUITE_TIMED_TIGHTPEGENCODER_H__
#include "TimedEncoder.h"
#include <rfb/RREEncoder.h>
#include <rfb/SConnection.h>

namespace suite {

  using namespace rfb;

  class TimedRREEncoder : public RREEncoder, public TimedEncoder
  {
  public:
    TimedRREEncoder(SConnection* conn_, Manager* manager);
    ~TimedRREEncoder();

    virtual void writeRect(const PixelBuffer* pb, const Palette& palette);
    virtual void writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour);
  };
}
#endif // __SUITE_TIMED_RREENCODER_H__
