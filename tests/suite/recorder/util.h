#ifndef __SUITE_RECORDER_UTIL
#define __SUITE_RECORDER_UTIL

#include "rfb/Rect.h"
#include <X11/Xlib.h>
#include <vector>
// Calculates the area covered by a vector of Rects, excluding any overlaps
int rectTotalArea(std::vector<rfb::Rect>& intersections);
rfb::Rect boundingRect(std::vector<rfb::Rect>& rects);
#endif // __SUITE_RECORDER_UTIL