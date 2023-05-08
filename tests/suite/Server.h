#ifndef __SUITE_SERVER_H__
#define __SUITE_SERVER_H__
#include "codec/timed/TimedEncoder.h"
#include "rfb/PixelFormat.h"
#include "streams/DummyInStream.h"
#include "streams/DummyOutStream.h"
#include "codec/Image.h"
#include "Manager.h"
#include "rfb/UpdateTracker.h"
#include "rfb/encodings.h"
#include <map>

namespace suite {

  static const rfb::PixelFormat fbPF(32, 24, false, true,
                                     255, 255, 255, 0, 8, 16);

  // Encodings to use
  static const rdr::S32 encodings[] = {
    rfb::encodingTight, rfb::encodingCopyRect, rfb::encodingRRE,
    rfb::encodingHextile, rfb::encodingZRLE, rfb::pseudoEncodingLastRect,
    rfb::pseudoEncodingQualityLevel0 + 8,
    rfb::pseudoEncodingCompressLevel0 + 2};

    class Server : public rfb::SConnection
    {
    public:
      Server(int width, int height, rfb::PixelFormat pf = fbPF);
      Server(int width, int height, std::array<EncoderClass,
                                               ENCODERS_COUNT> encoders,
                                    rfb::PixelFormat pf = fbPF);
      ~Server();

      void writeUpdate(const rfb::UpdateInfo& ui, const rfb::PixelBuffer* pb);

      // Unused
      virtual void setDesktopSize(int fb_width, int fb_height,
                                  const rfb::ScreenSet& layout);
      // Loads an Image onto the framebuffer at x, y
      virtual void loadImage(Image* image, int x = 0, int y = 0);

      std::map<EncoderClass, encoderStats> stats();
    public:
      DummyInStream *in;
      DummyOutStream *out;
    protected:
      Manager *manager;
      rfb::SimpleUpdateTracker updates;
      rfb::ManagedPixelBuffer* pb;
      void init(int width, int height, rfb::PixelFormat pf);
    };

  }
#endif // __SUITE_SERVER_H__
