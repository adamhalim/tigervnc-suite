#ifndef __SUITE_TIMEDENCODER_H__
#define __SUITE_TIMEDENCODER_H__

#include "../../stats/EncoderStats.h"
#include <rdr/InStream.h>
#include <rdr/MemOutStream.h>
#include <rdr/OutStream.h>
#include <rfb/Encoder.h>
#include <rfb/PixelBuffer.h>
#include <rfb/SConnection.h>
#include <chrono>
#include <stdexcept>
#include <string>
#include <vector>
namespace suite {

    // Copied from EncodeManager.cxx
  namespace enumEncoder {
    enum EncoderClass {
      encoderRaw,
      encoderRRE,
      encoderHextile,
      encoderTight,
      encoderTightJPEG,
      encoderZRLE,
      encoderClassMax,
    };

    enum PseudoEncodingLevel {
      NONE = -1,
      ZERO,
      ONE,
      TWO,
      THREE,
      FOUR,
      FIVE,
      SIX,
      SEVEN,
      EIGHT,
      NINE
    };

    struct EncoderSettings {
      EncoderClass encoderClass;
      int *rfbEncoding;
      size_t encodingSize;
      PseudoEncodingLevel quality;
      PseudoEncodingLevel compression;
      std::string name;
    };

    inline std::string encoderClasstoString(EncoderClass encoderClass)
    {
      switch (encoderClass) {
        case encoderRaw:
          return "Raw";
        case encoderRRE:
          return "RRE";
        case encoderHextile:
          return "Hextile";
        case encoderTight:
          return "Tight";
        case encoderTightJPEG:
          return "TightJPEG";
        case encoderZRLE:
          return "ZRLE";
        default:
          throw std::logic_error("EncoderClass not implemented");
      }
    }
  }
  using namespace enumEncoder;

  // Forward declarations
  class Manager;
  struct WriteUpdate;

  class TimedEncoder
  {
  public:
    TimedEncoder(EncoderClass encoderclass, Manager* manager);
    ~TimedEncoder();

    void startWriteRectTimer(rfb::SConnection* sconn);
    void stopWriteRectTimer(const rfb::PixelBuffer* pb);
    void startWriteSolidRectTimer(rfb::SConnection* sconn);
    void stopWriteSolidRectTimer(int width, int height);
    const EncoderClass encoderClass;
    EncoderStats* stats() { return _stats; };
    virtual bool isSupported() { return true; };
    virtual void writeRect(const rfb::PixelBuffer* pb,
                           const rfb::Palette& palette)=0;
    virtual void writeSolidRect(int width, int height,
                                const rfb::PixelFormat& pf,
                                const rdr::U8* colour)=0;
    uint currentWriteUpdate;
  private:
    rdr::MemOutStream *encoderOutstream;
    rdr::OutStream* os;
    rdr::InStream* is;
    rfb::SConnection* conn_;
    Manager* manager; // FIXME: Can we avoid needing a Manager* here?
    EncoderStats* _stats;
    std::chrono::system_clock::time_point writeRectStart;
    std::chrono::system_clock::time_point writeSolidRectStart;
  };
}
#endif // __SUITE_TIMEDENCODER_H__ 
