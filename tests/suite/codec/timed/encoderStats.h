#ifndef __SUITE_ENCODERSTATS_H__
#define __SUITE_ENCODERSTATS_H__

#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

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

    void print()
    {
      int tableWidth = 35;
      int precision = 5;
      std::cout << "\n\t" << name << " encoder: (seconds)\n\t\t"
                << std::setprecision(precision) << std::fixed 
                << std::setw(tableWidth) << std::setfill(' ') << std::left
                << "writeRect: " << std::right << writeRectEncodetime
                << "\n\t\t" << std::left  << std::setw(tableWidth)
                << "writeSolidRect: " << std::right 
                << compressionRatioSolidRects()
                << "\n\t\t" << std::setw(tableWidth) << std::left
                << "total: " << std::right << writeRectEncodetime 
                                            + writeSolidRectEncodetime
                << "\n\t\t" << std::setfill('-')
                << std::setw(tableWidth+precision+2)
                << std::left << "" << std::setfill(' ')
                << "\n\t\t" << std::setw(tableWidth) << std::left  
                << "# rects: " << std::right << nRects
                << "\n\t\t" << std::setw(tableWidth) << std::left
                << "# solidRects: " << std::right << nSolidRects 
                << "\n\t\t" << std::setfill('-') 
                << std::setw(tableWidth+precision+2)
                << std::left << "" << std::setfill(' ')
                << "\n\t\t" << std::setw(tableWidth) << std::left
                << "MPx/s (rects):" << std::right 
                << megaPixelsPerSecondRects() << "\n\t\t"
                << std::setw(tableWidth) << std::left
                << "MPx/s (solidRects:)" << std::right 
                << megaPixelsPerSecondSolidRects() << "\n\t\t"
                << std::setw(tableWidth) << std::left 
                << "MPx/s (combined)" << std::right
                << megaPixelsPerSecondCombined() << "\n\t\t"
                << std::setfill('-') 
                << std::setw(tableWidth+precision+2)
                << std::left << "" << std::setfill(' ') << "\n\t\t"
                << std::setw(tableWidth) << std::left
                << "Compression ratio rects: "
                << compressionRatioRects()
                << "\n\t\t" << std::setw(tableWidth) << std::left
                << "Compression ratio solidRects: " << std::right 
                << compressionRatioSolidRects() << "\n\t\t"
                << std::setw(tableWidth) << std::left
                << "Compression ratio combined: " 
                << compressionRatioCombined() << "\n\t\t"
                << std::setfill('-') << std::setw(tableWidth)
                << std::setw(tableWidth+precision+2)
                << std::left << "" << std::setfill(' ') << "\n\t\t"
                << std::setw(tableWidth) << std::left
                << "Score (time * compression ratio):" << std::right
                << score() << "\n\t\t" << std::setfill('-') 
                << std::setw(tableWidth+precision+2)
                << std::left << "" << std::endl;
    }
  };
}
#endif // __SUITE_ENCODERSTATS_H__
