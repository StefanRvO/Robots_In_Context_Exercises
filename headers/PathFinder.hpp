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
  private:
    point closestPoint;
    float closestDistance;
    std::vector<point> movepath;
    std::vector< std::vector< mapSpace > > *map = nullptr;
    unsigned long size_x;
    unsigned long size_y;
    point currentPoint;
    point goal;
    vector2D *m_line = nullptr;
    vector2D *cur_goal_vec = nullptr;
  public:
    PathFinder(std::vector< std::vector< mapSpace > > *_map);
    ~PathFinder();
    std::vector<point> getPath(point startpos, point endpos);
    void goToNextObstacle();
    void FindClosestPointOnObstacle();
    point FindNextPointOnObstacle(direction dir);
    point FindNextPointOnLine(bool *obstacle);
    int findNextLeavePoint(std::vector <point> &obstaclePoints);

};
