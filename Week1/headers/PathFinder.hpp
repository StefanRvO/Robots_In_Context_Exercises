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
    //point closestPoint;
    //float closestDistance;
    std::vector<point> movepath;
    std::vector< std::vector< mapSpace > > &map;
    long size_x;
    long size_y;
    point currentPoint;
    vector2D *m_line = nullptr;
    virtual void findPath() {};
    virtual void goToNextObstacle() {};
    virtual void findNextLeavePoint() {};
    virtual point FindNextPointOnObstacle(direction dir, point lastpoint);
    virtual point FindNextPointOnLine(bool *obstacle, vector2D &line);
    virtual int findClosestPoint(std::vector <point> &obstaclePoints, const point &goal_p);

  public:
    PathFinder(std::vector< std::vector< mapSpace > > &_map);
    virtual ~PathFinder();
    virtual std::vector<point> getPath(point startpos, point endpos);
    //virtual point FindClosestObstacle(point pos);

};
