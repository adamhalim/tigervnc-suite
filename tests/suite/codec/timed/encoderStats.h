#ifndef __SUITE_ENCODERSTATS_H__
#define __SUITE_ENCODERSTATS_H__

#include <algorithm>
#include <cmath>
#include <map>
#include <string>
#include <vector>
#include <iomanip>
#include <iostream>

namespace suite {

  struct writeUpdateStats
  {
    uint timeRequired;
    double timeSpent;

    // Was the frame encoded within the time budget?
    bool encodedInTime() const { return timeSpent <= timeRequired; }
    // How much time (in milliseconds) do we have left, or did we surpass
    // the budget with?
    int encodingMargin() const { return timeRequired - timeSpent; }

  };

  struct writeRectStats {
    double timeSpent;
    int pixelCount;
    double delayedFrames;

    friend bool operator<(const writeRectStats& lhs, const writeRectStats& rhs)
    {
      return lhs.timeSpent < rhs.timeSpent;
    }
  };

  struct writeRectsStats {
    std::vector<writeRectStats> writeRects;
    std::vector<writeRectStats> writeSolidRects;
  };

  struct writeUpdateStatistics {
    double meanWriteUpdate;
    double medianWriteRect;
    double varianceWriteUpdate;
    double stdWriteUpdate;
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
    double delayedFrames;
    std::string name;
    std::vector<writeUpdateStats> framesData;     // EncodeManager::writeUpdate
    std::map<uint,writeRectsStats> writeUpdates;  // Encoder::writeRect
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

    double delayedFramesRatio()
    {
      return delayedFrames * 100 / framesData.size();
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
        variance += (framesData[i].timeSpent - mean)
                  * (framesData[i].timeSpent - mean);

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

  private:
    // --- writeRectStats statistics functions ---

    struct writeUpdateStatistics writeUpdateStatistics()
    {
      std::vector<double> mPxPerSecond;
      double sumMPxPerSecond = 0;
      for (const auto& update : writeUpdates) {
        double timeSum = 0;
        double pixels = 0;

        // Sum all updates belonging to the same writeUpdate
        for (const auto& rects : update.second.writeRects) {
          timeSum += rects.timeSpent;
          pixels += rects.pixelCount;
        }

        for (const auto& solidRects : update.second.writeSolidRects) {
          timeSum += solidRects.timeSpent;
          pixels += solidRects.pixelCount;
        }

        double mps = pixels / (timeSum * 10e6);
        sumMPxPerSecond += mps;
        mPxPerSecond.push_back(mps);
      }

      std::nth_element(mPxPerSecond.begin(),
                       mPxPerSecond.begin() + 1, mPxPerSecond.end());
      double median = mPxPerSecond[mPxPerSecond.size() / 2];

      double mean = sumMPxPerSecond / mPxPerSecond.size();
      double variance = 0;
      for (uint i = 0; i < mPxPerSecond.size(); i++)
        variance += (mPxPerSecond[i] - mean) * (mPxPerSecond[i] - mean);

      variance /= (mPxPerSecond.size() - 1);
      
      struct writeUpdateStatistics stats {
        .meanWriteUpdate = mean,
        .medianWriteRect = median,
        .varianceWriteUpdate = variance,
        .stdWriteUpdate = std::sqrt(variance),
      };
      return stats;
    }

    // writeRectStats functions end //

  public:
    void print()
    {
      int tableWidth = 40;
      int precision = 5;
      struct writeUpdateStatistics stats = writeUpdateStatistics();
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
                << std::left << "" << "\n\t\t" << std::setw(tableWidth)
                << std::setfill(' ')
                << "EncodeManager writeUpdate mean (ms):" << std::right 
                <<  meanTimeSpent() << "\n\t\t" << std::left 
                << std::setfill(' ') << std::setw(tableWidth)
                << "EncodeManager writeUpdate median:" << std::right 
                << medianTimeSpent() << "\n\t\t"
                << std::left << std::setfill(' ') << std::setw(tableWidth)
                << "EncodeManager writeUpdate variance:" << std::right 
                << varianceTimeSpent() << "\n\t\t" << std::left 
                << std::setfill(' ') << std::setw(tableWidth)
                << "EncodeManager writeUpdate mean margin:" << std::right 
                << meanEncodingMargin() << "\n\t\t" << std::setw(tableWidth)
                << std::setfill(' ') << std::left 
                << "EncodeManager delayed frames (%):" << delayedFramesRatio()
                << "\n\t\t" << std::setfill('-') 
                << std::setw(tableWidth+precision+2)
                << std::left << "" << "\n\t\t" << std::setw(tableWidth)
                << std::setfill(' ')
                << "Encoder writeUpdate mean (MPx/s):" << std::right
                << stats.meanWriteUpdate << "\n\t\t" << std::left 
                << std::setfill(' ') << std::setw(tableWidth)
                << "Encoder writeUpdate median:" << std::right
                << stats.medianWriteRect << "\n\t\t"
                << std::left << std::setfill(' ') << std::setw(tableWidth)
                << "Encoder writeUpdate variance:" << std::right 
                << stats.varianceWriteUpdate
                << std::endl;
    }
  };
}
#endif // __SUITE_ENCODERSTATS_H__
