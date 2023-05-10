#ifndef __SUITE_TIMEDENCODER_H__
#define __SUITE_TIMEDENCODER_H__

#include "rdr/InStream.h"
#include "rdr/MemOutStream.h"
#include "rdr/OutStream.h"
#include "rfb/Encoder.h"
#include "rfb/PixelBuffer.h"
#include "rfb/SConnection.h"
#include <chrono>
#include <stdexcept>
#include <string>
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
      int rfbEncoding;
      PseudoEncodingLevel quality;
      PseudoEncodingLevel compression;
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

  struct encoderStats {
    double writeRectEncodetime;
    double writeSolidRectEncodetime;
    int inputSizeRects;
    int outputSizeRects;
    int inputSizeSolidRects;
    int outputSizeSolidRects;
    int nRects;
    int nSolidRects;
    std::string name;

    double compressionRatioRects() 
    {
      return (double) outputSizeRects / inputSizeRects; 
    }

    double compressionRatioSolidRects()
    {
       return (double) outputSizeSolidRects / inputSizeSolidRects;
    }

    double compressionRatioCombined()
    {
      return (double) (outputSizeRects + outputSizeSolidRects)
                    / (inputSizeRects + inputSizeSolidRects);
    }

    double megaPixelsPerSecondRects()
    {
      return inputSizeRects / (writeRectEncodetime * 10e6);
    }
  
    double megaPixelsPerSecondSolidRects()
    {
      return inputSizeSolidRects / (writeSolidRectEncodetime * 10e6);
    }

    double megaPixelsPerSecondCombined()
    {
      return (inputSizeRects + inputSizeSolidRects) 
           / ((writeRectEncodetime + writeSolidRectEncodetime) * 10e6);
    }

    // Returns a "score" which is a function of time 
    // per compressed data
    double score()
    {
      return (writeRectEncodetime + writeSolidRectEncodetime)
           * (compressionRatioCombined());
    }
  };

  class TimedEncoder 
  {
  public:
    TimedEncoder(EncoderClass encoderclass);
    ~TimedEncoder();

    void startWriteRectTimer(rfb::SConnection* sconn);
    void stopWriteRectTimer(const rfb::PixelBuffer* pb);
    void startWriteSolidRectTimer(rfb::SConnection* sconn);
    void stopWriteSolidRectTimer(int width, int height);
    const EncoderClass encoderClass;
    encoderStats stats() { return stats_; };
  protected:
    rdr::MemOutStream *encoderOutstream;
    rdr::OutStream* os;
    rdr::InStream* is;
    rfb::SConnection* conn_;
  private:
    encoderStats stats_;
    std::chrono::system_clock::time_point writeRectStart;
    std::chrono::system_clock::time_point writeSolidRectStart;
  };
}

#endif // __SUITE_TIMEDENCODER_H__ 
