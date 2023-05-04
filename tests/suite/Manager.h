#ifndef __SUITE_MANAGER_H__
#define __SUITE_MANAGER_H__
#include "rfb/EncodeManager.h"
#include "rfb/SConnection.h"

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

    class Manager : public rfb::EncodeManager 
    {
    public:
        Manager(class rfb::SConnection *conn);
        ~Manager();
    };
}
#endif // __SUITE_MANAGER_H__
