#include "util.h"

int rectTotalArea(std::vector<rfb::Rect>& rects)
{
  int intersectonArea = 0;
  for (unsigned int i = 0; i < rects.size() - 1; i++) {
    rfb::Rect first = rects[i];
    intersectonArea += first.area();
    for (unsigned int j = i + 1; j < rects.size(); j++) {
      rfb::Rect second = rects[j];
      rfb::Rect intersection = first.intersect(second);
      // Don't caclulate the overlapping intersecting area twice
      if (intersection.area()) {
        intersectonArea -= intersection.area();
        // Covers cases where rects are completely within
        // the boundaries of another rect
        if (intersectonArea < 0)
          intersectonArea = 0;
      }
    }
  }
  // Include last rect's area
  intersectonArea += rects.back().area();
  return intersectonArea;
}
