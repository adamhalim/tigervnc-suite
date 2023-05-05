#ifndef __SUITE_MANAGER_H__
#define __SUITE_MANAGER_H__
#include "codec/timed/TimedEncoder.h"
#include "rfb/EncodeManager.h"
#include "rfb/SConnection.h"
#include "rfb/encodings.h"
#include <map>
#include <stdexcept>
#include <string>

namespace suite {
    

  // Copied from EncodeManager.cxx
  enum EncoderClass {
    encoderRaw,
    encoderRRE,
    encoderHextile,
    encoderTight,
    encoderTightJPEG,
    encoderZRLE,
    encoderClassMax,
  };

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

    class Manager : public rfb::EncodeManager 
    {
    public:
      Manager(class rfb::SConnection *conn);
      ~Manager();

      std::map<const int, encoderStats> stats();
    protected:
      std::map<const int, TimedEncoder*> timedEncoders;
    };
}
#endif // __SUITE_MANAGER_H__
