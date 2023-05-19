#include "util.h"
#include "rfb/Region.h"

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
  for (rfb::Rect rect : rects)
    boundingRect = boundingRect.union_boundary(rect);
  return boundingRect;
}

