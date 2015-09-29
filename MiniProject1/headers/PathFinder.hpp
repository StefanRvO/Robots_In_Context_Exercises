#pragma once
#include "../headers/Map.hpp"
#include "../headers/vector2D.hpp"
#include <vector>

enum direction
{
  clockwise = 0,
  counterclockwise = 1,
};

class PathFinder
{
  protected:
    std::vector< std::vector< mapSpace > > &map;
    long size_x;
    long size_y;
  public:
    PathFinder(std::vector< std::vector< mapSpace > > &_map);
    virtual ~PathFinder();
    virtual point FindClosestObstacle(const point &pos) const;

};
