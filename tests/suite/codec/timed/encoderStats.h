#ifndef __SUITE_ENCODERSTATS_H__
#define __SUITE_ENCODERSTATS_H__

#include <algorithm>
#include <cmath>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

namespace suite {

  struct writeUpdateStats
  {
    uint timeRequired;
    uint timeSpent;

    // Was the frame encoded within the time budget?
    bool encodedInTime() const { return timeSpent <= timeRequired; }
    // How much time (in milliseconds) do we have left, or did we surpass
    // the budget with?
    int encodingMargin() const { return timeRequired - timeSpent; }

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
    std::vector<writeUpdateStats> framesData;
    static const int BPP = 4;

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
      return inputSizeRects / (writeRectEncodetime * 10e6 * BPP);
    }
  
    double megaPixelsPerSecondSolidRects()
    {
      return inputSizeSolidRects / (writeSolidRectEncodetime * 10e6 * BPP);
    }

    double megaPixelsPerSecondCombined()
    {
      return (inputSizeRects + inputSizeSolidRects) 
           / ((writeRectEncodetime + writeSolidRectEncodetime) * 10e6 * BPP);
    }

    // Returns a "score" which is a function of time 
    // per compressed data
    double score()
    {
      return (writeRectEncodetime + writeSolidRectEncodetime)
           * (compressionRatioCombined());
    }
  
  private:
    // --- frameData statistics functions ----
    // Note that these are EncodeManager specfic, which include
    // RFB protocol overhead.

    double medianTimeSpent()
    {
      return medianFrameDataValue([](const writeUpdateStats& lhs,
                                     const writeUpdateStats& rhs) {
        return lhs.timeSpent < rhs.timeSpent;
      });
    }

    double medianTimeRequired()
    {
      return medianFrameDataValue([](const writeUpdateStats& lhs,
                                     const writeUpdateStats& rhs) {
        return lhs.timeRequired < rhs.timeRequired;
      });
    }

    double medianEncodingtime()
    {
      return medianFrameDataValue([](const writeUpdateStats& lhs,
                                     const writeUpdateStats& rhs) {
        return lhs.encodingMargin() < rhs.encodingMargin();
      });
    }

    double medianFrameDataValue(bool comparator(const writeUpdateStats& lhs,
                                                const writeUpdateStats& rhs))
    {
      std::vector<writeUpdateStats> copy = framesData;
      std::nth_element(copy.begin(), copy.begin() + 1, copy.end(), comparator);
      return copy[(int) (copy.size() / 2)].timeSpent;
    }


    double meanTimeSpent()
    {
      double sum = 0;
      for(writeUpdateStats& f : framesData)
        sum += f.timeSpent;
      return sum / framesData.size();
    }

    double meanTimeRequired()
    {
      double sum = 0;
      for(writeUpdateStats& f : framesData)
        sum += f.timeRequired;
      return sum / framesData.size();
    }

    double meanEncodingMargin()
    {
      double sum = 0;
      for(writeUpdateStats& f : framesData)
        sum += f.encodingMargin();
      return sum / framesData.size();
    }

    double varianceTimeSpent()
    {
      const double mean = meanTimeSpent();
      const int size = framesData.size();
      double variance = 0;
      for(int i = 0; i < size; i++) 
        variance += (framesData[i].timeSpent - mean) * (framesData[i].timeSpent - mean);

      // divide by N-1 for unbiased variance
      // https://en.wikipedia.org/wiki/Bessel%27s_correction
      variance /= (size - 1); 
      return variance;
    }

    double varianceTimeRequired()
    {
      const double mean = meanTimeRequired();
      const int size = framesData.size();
      double variance = 0;
      for(int i = 0; i < size; i++) 
        variance += (framesData[i].timeRequired - mean)
                  * (framesData[i].timeRequired - mean);

      variance /= (size - 1); 
      return variance;
    }

    double varianceEncodingMargin()
    {
      const double mean = meanEncodingMargin();
      const int size = framesData.size();
      double variance = 0;
      for(int i = 0; i < size; i++) 
        variance += (framesData[i].encodingMargin() - mean)
                  * (framesData[i].encodingMargin() - mean);

      variance /= (size - 1); 
      return variance;
    }

    double stdTimeSpent() { return std::sqrt(varianceTimeSpent()); }
    double stdTimeRequired() { return std::sqrt(varianceTimeRequired()); }
    double stdEncodingMargin() { return std::sqrt(varianceEncodingMargin()); }

    // writeUpdateStats functions end //

  public:
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
