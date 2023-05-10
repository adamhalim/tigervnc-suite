#ifndef __SUITE_ENCODERSTATS_H__
#define __SUITE_ENCODERSTATS_H__

#include <string>
#include <vector>

namespace suite {

  struct frameData
  {
    uint timeRequired;
    uint timeSpent;
  };

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
    std::vector<frameData> framesData;

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
}
#endif // __SUITE_ENCODERSTATS_H__
