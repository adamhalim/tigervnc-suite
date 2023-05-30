#ifndef __SUITE_RECORDER_UTIL
#define __SUITE_RECORDER_UTIL

#include "rfb/Rect.h"
#include <X11/Xlib.h>
#include <vector>

// Represents how much data is lost for a sequence of rfb::Rects,
// as well as how much unneded data is captured by the bounding rectangle
// covering the sequence of rfb::Rects
struct IntersectionStats {
  int lostDataArea;
  int overDimensionedArea;
};

// Calculates the area covered by a vector of Rects, excluding any overlaps
int rectTotalArea(std::vector<rfb::Rect>& intersections);
IntersectionStats detectInteresctions(std::vector<rfb::Rect>& rects);
rfb::Rect boundingRect(std::vector<rfb::Rect>& rects);
#endif // __SUITE_RECORDER_UTIL