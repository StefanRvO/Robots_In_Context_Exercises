#include "../headers/Bug2.hpp"
#include <iostream>
#include <cassert>
Bug2::~Bug2()
{
}

void Bug2::findPath()
{
  prev_leave_point = m_line->getStartPoint();
  while(currentPoint != m_line->getEndPoint())
  {
    goToNextObstacle();
    findNextLeavePoint();
  }

}

void Bug2::goToNextObstacle()
{
  bool hitObstacle = false;
  while(!hitObstacle)
  {
    point p = FindNextPointOnLine(&hitObstacle, *m_line);
    currentPoint = p;
    movepath.push_back(p);
    if(currentPoint == m_line->getEndPoint()) return;
  }
  movepath.pop_back();
  currentPoint = movepath.back();
}

void Bug2::findNextLeavePoint()
{
  //std::cout << "Hit obstacle" << std::endl;
  direction dir = static_cast<direction>(rand()%2);
  std::vector <point> obstaclePoints;

  //go all around the obstacle, until we reach the m_line
  obstaclePoints.push_back(currentPoint);
  point lastpoint = {-1, -1};

  while(true) //kinda ugly, but this is easier. Should probably be rewritten
  {
    if(currentPoint == m_line->getEndPoint()) return;
    point p = FindNextPointOnObstacle(dir, lastpoint);
    lastpoint = currentPoint;
    currentPoint = p;
    obstaclePoints.push_back(p);
    assert (map[p.x][p.y] != mapSpace::obstacle);
    float distance = m_line->distanceToPoint(p);
    if(distance < 1.) //we have hit the m_line
    {
      //check if a neighbour is closer to the m_line, if so, go to that instead
      point closestpoint = p;
      float closestDistance = distance;
      for(int i = -1; i <= 1; i++)
      {
        for(int j = -1; j <=1; j++)
        {
          if((j == 0 && i == 0)) continue;
          point p1 = {p.x + j, p.y + i};
          if(p.y < 0 || p.x < 0 || p.y > size_y || p.x > size_x ||
              map[p1.x][p1.y] == mapSpace::obstacle) continue;

          if(m_line->distanceToPoint(p1) < closestDistance)
          {
            closestpoint = p1;
            closestDistance = m_line->distanceToPoint(p1);
          }
        }
      }
      if(p != closestpoint) obstaclePoints.push_back(closestpoint);
      if(m_line->getEndPoint().GetDistance(obstaclePoints.back()) < m_line->getEndPoint().GetDistance(prev_leave_point))
        break;
    }
  }
  movepath.insert(movepath.end(), obstaclePoints.begin(), obstaclePoints.end());

  prev_leave_point = movepath.back();
  currentPoint = movepath.back();
  //std::cout << "left obstacle" << std::endl;
}
