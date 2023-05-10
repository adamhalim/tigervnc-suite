#ifndef __SUITE_MANAGER_H__
#define __SUITE_MANAGER_H__
#include "codec/timed/TimedEncoder.h"
#include "rfb/EncodeManager.h"
#include "rfb/SConnection.h"
#include "rfb/encodings.h"
#include "codec/timed/encoderStats.h"
#include <map>
#include <stdexcept>
#include <string>
#include <array>

namespace suite {

  using namespace enumEncoder;

  // Copied from EncodeManager.cxx
  inline const char *encoderClassName(EncoderClass klass)
  {
    switch (klass) {
    case encoderRaw:
      return "Raw";
    case encoderRRE:
      return "RRE";
    case encoderHextile:
      return "Hextile";
    case encoderTight:
      return "Tight";
    case encoderTightJPEG:
      return "Tight (JPEG)";
    case encoderZRLE:
      return "ZRLE";
    case encoderClassMax:
      break;
    }

    return "Unknown Encoder Class";
  }

  inline std::string encodingToString(const int encoding)
  {
    switch (encoding) {
      case rfb::encodingRaw:
        return "Raw";
      case rfb::encodingCopyRect:
        return "CopyRect";
      case rfb::encodingRRE:
         return "RRE";
      case rfb::encodingCoRRE:
        return "CoRRE";
      case rfb::encodingHextile:
        return "Hextile";
      case rfb::encodingTight:
        return "Tight";
      case rfb::encodingZRLE:
        return "ZRLE";
    }
    return std::to_string(encoding);
  }

    static const int ENCODERS_COUNT = 6;

    class Manager : public rfb::EncodeManager 
    {
    public:
      Manager(class rfb::SConnection *conn);
      // Creates an EncodingManager that only uses the specified Encoder
      // for all encodings.
      Manager(class rfb::SConnection *conn, EncoderSettings settings);
      ~Manager();

      void writeUpdate(const rfb::UpdateInfo& ui, const rfb::PixelBuffer* pb,
                       const rfb::RenderedCursor* renderedCursor);

      void writeUpdate(const rfb::UpdateInfo& ui, const rfb::PixelBuffer* pb,
                       const rfb::RenderedCursor* renderedCursor,
                       uint frameTime);

      std::map<EncoderClass, encoderStats> stats();
    protected:
      std::map<EncoderClass, TimedEncoder*> timedEncoders;
      const bool SINGLE_ENCODER;
    private:
      TimedEncoder* timedEncoder_;
    };
}
#endif // __SUITE_MANAGER_H__
