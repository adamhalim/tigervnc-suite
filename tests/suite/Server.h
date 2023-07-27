#ifndef __SUITE_SERVER_H__
#define __SUITE_SERVER_H__
#include "codec/timed/TimedEncoder.h"
#include "stats/ManagerStats.h"
#include "streams/DummyInStream.h"
#include "streams/DummyOutStream.h"
#include "codec/Image.h"
#include "Manager.h"
#include <rfb/PixelFormat.h>
#include <rfb/UpdateTracker.h>
#include <rfb/encodings.h>
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

  static const EncoderSettings defaultEncoderSettings = {
    .encoderClass = enumEncoder::encoderTight,
    .rfbEncoding = (int*) encodings,
    .encodingSize = sizeof(encodings) / sizeof(*encodings),
    .quality = enumEncoder::EIGHT,
    .compression = enumEncoder::TWO,
    .name = "Default",
  };

    class Server : public rfb::SConnection
    {
    public:
      Server(int width, int height, rfb::PixelFormat pf = fbPF,
             bool debug = false);
      Server(int width, int height, EncoderSettings settings,
                                    rfb::PixelFormat pf = fbPF);
      ~Server();

      void writeUpdate(const rfb::UpdateInfo& ui, const rfb::PixelBuffer* pb);

      // Unused
      virtual void setDesktopSize(int fb_width, int fb_height,
                                  const rfb::ScreenSet& layout);
      // Loads an Image onto the framebuffer at x, y
      virtual void loadImage(const Image* image, int x = 0, int y = 0);

      ManagerStats stats();
      EncoderSettings settings;
    public:
      DummyInStream *in;
      DummyOutStream *out;
      Manager *manager;
    protected:
      rfb::SimpleUpdateTracker updates;
      rfb::ManagedPixelBuffer* pb;
      void init(int width, int height, rfb::PixelFormat pf);
    };

  }
#endif // __SUITE_SERVER_H__
