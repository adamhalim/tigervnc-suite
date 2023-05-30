#ifndef __SUITE_TIMED_TIGHTJPEGENCODER_H__
#define __SUITE_TIMED_TIGHTJPEGENCODER_H__
#include "TimedEncoder.h"
#include "rfb/TightJPEGEncoder.h"
#include "rfb/SConnection.h"
namespace suite {

  using namespace rfb;

  class TimedTightJPEGEncoder : public TightJPEGEncoder, public TimedEncoder
  {
  public:
    TimedTightJPEGEncoder(SConnection* conn_, Manager* manager);
    ~TimedTightJPEGEncoder();

    virtual void writeRect(const PixelBuffer* pb, const Palette& palette);
    virtual void writeSolidRect(int width, int height,
                                const PixelFormat& pf,
                                const rdr::U8* colour);
  };

}

#endif // __SUITE_TIMED_TIGHTJPEGENCODER_H__
