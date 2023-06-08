#ifndef __SUITE_RECORDER_UTIL
#define __SUITE_RECORDER_UTIL

#include "rfb/Rect.h"
#include <X11/Xlib.h>
#include <vector>

// Represents how much data is lost for a sequence of rfb::Rects,
// as well as how much unneded data is captured by the bounding rectangle
// covering the sequence of rfb::Rects
struct ImageUpdateStats {
  int lostDataArea;
  int overDimensionedArea;
  double encodingTime;
  double margin;
};

// Calculates the area covered by a vector of Rects, excluding any overlaps
int rectTotalArea(std::vector<rfb::Rect>& intersections);
void detectInteresctions(std::vector<rfb::Rect>& rects,
                         ImageUpdateStats& stats);
rfb::Rect boundingRect(std::vector<rfb::Rect>& rects);
#endif // __SUITE_RECORDER_UTIL