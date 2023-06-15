#include "util.h"
#include <rfb/Region.h>

int rectTotalArea(std::vector<rfb::Rect>& rects)
{
  rfb::Region region;
  for (uint i = 0; i < rects.size(); i++) {
    region.assign_union(rects[i]);
  }

  std::vector<rfb::Rect> regionRects;
  region.get_rects(&regionRects);
  int area = 0;
  for (uint i = 0; i < regionRects.size(); i++) {
    area += regionRects[i].area();
  }
  return area;
}

rfb::Rect boundingRect(std::vector<rfb::Rect>& rects)
{
  rfb::Rect boundingRect;
  for (unsigned int i = 0; i < rects.size(); i++)
    boundingRect = boundingRect.union_boundary(rects[i]);
  return boundingRect;
}

// Calculates the IntersectionStats for a vector of rfb::Rects
void detectInteresctions(std::vector<rfb::Rect>& rects,
                         ImageUpdateStats& stats)
{
  int interesectArea = 0;
  // Loop rects all events and calculate the intersections between them.
  for (uint i = 0; i < rects.size() - 1; i++) {
    rfb::Rect first = rects[i];
    for (uint j = i + 1; j < rects.size(); j++) {
      rfb::Rect second = rects[j];
      rfb::Rect intersection = first.intersect(second);
      interesectArea += intersection.area();
    }
  }

  // Now, we want to grab the area of all rects in union, excluding
  // any overlapping areas.
  int rectAreaNoOverlap = 0;
  rectAreaNoOverlap = rectTotalArea(rects);

  int totalArea = boundingRect(rects).area();

  stats.lostDataArea = interesectArea;
  stats.overDimensionedArea = totalArea - rectAreaNoOverlap;
}
