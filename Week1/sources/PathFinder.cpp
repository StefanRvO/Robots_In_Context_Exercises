#include "../headers/PathFinder.hpp"
#include <iostream>
#include <limits>
#include <cassert>
PathFinder::PathFinder(std::vector< std::vector< mapSpace > > *_map)
{
  map = _map;
  size_x = map->size();
  size_y = (*map)[0].size();
}
PathFinder::~PathFinder()
{
  if(m_line != nullptr) delete m_line;
}

std::vector<point> PathFinder::getPath(point startpos, point endpos)
{
  movepath.clear();
  movepath.push_back(startpos);
  if(m_line != nullptr) delete m_line;
  m_line = new vector2D(startpos, endpos);
  currentPoint = startpos;
  findPath();
  return movepath;
}


int PathFinder::findClosestPoint(std::vector <point> &obstaclePoints, const point &goal_p)
{
  int i = 0;
  float mindistance = std::numeric_limits<float>::infinity();
  int bestPoint = 0;
  for(auto &p : obstaclePoints)
  {
    float distance = p.GetDistance(goal_p);
    if(distance < mindistance)
    {
      bestPoint = i;
      mindistance = distance;
    }
    i++;
  }
  return bestPoint;
}

point PathFinder::FindNextPointOnLine(bool *obstacle, vector2D &line)
{
  //check all 8 directions collect the ones which are closer to the goal
  //than the current.
  point closestpoint = {0, 0};
  float curdistance = currentPoint.GetDistance(m_line->getEndPoint());
  float mindistance = std::numeric_limits<float>::infinity();
  for(int i = -1; i <= 1; i++)
  {
    for(int j = -1; j <=1; j++)
    {
      if(j == 0 && i == 0) continue;
      point this_point = currentPoint + (point){i, j};
      if(this_point.y < 0 || this_point.x < 0 ||
          this_point.y > size_y || this_point.x > size_x) continue;
      if(this_point.GetDistance(m_line->getEndPoint()) <= curdistance)
      {   //select the wanted point as the one closest to the straight line to goal.
        float distance_to_line = line.distanceToPoint(this_point);
        if(distance_to_line < mindistance)
        {
          closestpoint = this_point;
          mindistance = distance_to_line;
        }
      }
    }
  }
  if((*map)[closestpoint.x][closestpoint.y] == mapSpace::obstacle)
    *obstacle = true;
  return closestpoint;
}

point PathFinder::FindNextPointOnObstacle(direction dir)
{
  //std::cout << "r" << currentPoint.x << "\t" << currentPoint.y << std::endl;
  //sequencies of points to check
  static point counterclockwisesequence[] =          {{0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};
  static point clockwisequence[] =                   {{-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}};

  point *sequence_ptr = nullptr;
  if(dir == direction::clockwise) sequence_ptr = clockwisequence;
  else if(dir == direction::counterclockwise) sequence_ptr = counterclockwisesequence;

  //first, detect in which direction the obstacle is
  point p = currentPoint;
  unsigned int i;
  for(i = 0; (*map)[p.x][p.y] != mapSpace::obstacle; i++)
  {
    p = currentPoint + *(sequence_ptr + i);
    assert(i < sizeof(clockwisequence) / sizeof(clockwisequence[0]));
  }
  //Now, go from this point in the sequence and forward(even starting the sequence over),
  //until a free space is found
  while((*map)[p.x][p.y] != mapSpace::freespace)
  {
      i %= sizeof(clockwisequence) / sizeof(clockwisequence[0]);
      p = currentPoint + *(sequence_ptr + i);
      i++;
  }
  return p;

}
