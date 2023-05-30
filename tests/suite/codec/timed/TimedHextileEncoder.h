#ifndef __SUITE_TIMED_HEXTILEENCODER_H__
#define __SUITE_TIMED_HEXTILEENCODER_H__
#include "TimedEncoder.h"
#include "rfb/HextileEncoder.h"
#include "rfb/SConnection.h"
namespace suite {

  using namespace rfb;

  class TimedHextileEncoder : public HextileEncoder, public TimedEncoder
  {
  public:
    TimedHextileEncoder(SConnection* conn_, Manager* manager);
    ~TimedHextileEncoder();

    virtual void writeRect(const PixelBuffer* pb, const Palette& palette);
    virtual void writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour);
  };

}

#endif // __SUITE_TIMED_HEXTILEENCODER_H__
