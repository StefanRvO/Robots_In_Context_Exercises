#include "../headers/PathFinder.hpp"
#include <iostream>
#include <limits>
#include <cassert>
PathFinder::PathFinder(std::vector< std::vector< mapSpace > > &_map)
:map(_map)
{
  size_x = map.size();
  size_y = map[0].size();
}
PathFinder::~PathFinder()
{
}

point PathFinder::FindClosestObstacle(const point &pos) const
{
  //Search around the position and find the closest obstacle.
  std::vector<point> searchedpoints;
  point closestpoint = {9999999, 9999999};
  double closestDistance = std::numeric_limits<double>::infinity();
  int searchdistance = 1;
  while(searchdistance < pos.GetDistance(closestpoint))
  {   //Search around the position until we find an obstacle
    //first, find min and max searchdistances, so we don't search outside the map.
    int min_x = - searchdistance;
    if(min_x + pos.x < 0) min_x = -pos.x;
    int max_x = searchdistance;
    if( max_x + pos.x >= size_x) max_x = size_x - pos.x - 1;
    int min_y = - searchdistance;
    if(min_y + pos.y < 0) min_y = -pos.y;
    int max_y = searchdistance;
    if( max_y + pos.y >= size_y) max_y = size_y - pos.y - 1;

    //upper points
    if(pos.y + searchdistance < size_y)
    {
      for(int i = min_x; i <= max_x; i++)
      {
        point p = pos + (point){i, searchdistance};
        searchedpoints.push_back(p);
        double p_distance = pos.GetDistance(p);
        if(map[p.x][p.y] == mapSpace::obstacle && p_distance < closestDistance)
        {
          closestDistance = p_distance;
          closestpoint = p;
        }
      }
    }
    //lower points
    if(pos.y - searchdistance >= 0)
    {
      for(int i = min_x; i <= max_x; i++)
      {
        point p = pos + (point){i, -searchdistance};
        searchedpoints.push_back(p);
        double p_distance = pos.GetDistance(p);
        if(map[p.x][p.y] == mapSpace::obstacle && p_distance < closestDistance)
        {
          closestDistance = p_distance;
          closestpoint = p;
        }
      }
    }
    //left points
    if(pos.x - searchdistance >= 0)
    { //add 1 to initial and substract 1 from end, so we don't check corners twice
      for(int i = min_y + 1; i <= max_y - 1; i++)
      {
        point p = pos + (point){-searchdistance, i};
        searchedpoints.push_back(p);
        double p_distance = pos.GetDistance(p);
        if(map[p.x][p.y] == mapSpace::obstacle && p_distance < closestDistance)
        {
          closestDistance = p_distance;
          closestpoint = p;
        }
      }
    }
    //right points
    if(pos.x + searchdistance < size_x)
    { //add 1 to initial and substract 1 from end, so we don't check corners twice
      for(int i = min_y + 1; i <= max_y - 1; i++)
      {
        point p = pos + (point){searchdistance, i};
        searchedpoints.push_back(p);
        double p_distance = pos.GetDistance(p);
        if(map[p.x][p.y] == mapSpace::obstacle && p_distance < closestDistance)
        {
          closestDistance = p_distance;
          closestpoint = p;
        }
      }
    }
    searchdistance++;
  }
  return closestpoint;
}
