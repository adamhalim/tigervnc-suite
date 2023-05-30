#ifndef __SUITE_TIMED_TIGHTENCODER_H__
#define __SUITE_TIMED_TIGHTENCODER_H__

#include "TimedEncoder.h"
#include "rfb/EncodeManager.h"
#include "rfb/SConnection.h"
#include "rfb/TightEncoder.h"

namespace suite {

  using namespace rfb;

  class TimedTightEncoder : public TightEncoder, public TimedEncoder
  {
  public:
    TimedTightEncoder(SConnection* conn_, Manager* manager);
    ~TimedTightEncoder();

    virtual void writeRect(const PixelBuffer* pb, const Palette& palette);
    virtual void writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour);
  };
}

#endif // __SUITE_TIGHTENCODER_H__
