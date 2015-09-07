#include "../headers/Bug1.hpp"
#include <iostream>
#include <cassert>
Bug1::~Bug1()
{
  if(cur_goal_vec != nullptr) delete cur_goal_vec;
}

void Bug1::findPath()
{
  if(cur_goal_vec != nullptr) delete cur_goal_vec;
  cur_goal_vec = new vector2D(m_line->getStartPoint(), m_line->getEndPoint());
  while(currentPoint != m_line->getEndPoint())
  {
    goToNextObstacle();
    findNextLeavePoint();
    delete cur_goal_vec;
    cur_goal_vec = new vector2D(currentPoint, m_line->getEndPoint());
  }

}

void Bug1::goToNextObstacle()
{
  bool hitObstacle = false;
  while(!hitObstacle)
  {
    point p = FindNextPointOnLine(&hitObstacle, *cur_goal_vec);
    currentPoint = p;
    movepath.push_back(p);
    if(currentPoint == m_line->getEndPoint()) return;
  }
  movepath.pop_back();
  currentPoint = movepath.back();
}

void Bug1::findNextLeavePoint()
{
  //std::cout << "Hit obstacle" << std::endl;
  direction dir = static_cast<direction>(rand()%2);
  std::vector <point> obstaclePoints;

  //go all around the obstacle
  point lastpoint = {-1, -1};
  obstaclePoints.push_back(currentPoint);
  do
  {
    if(currentPoint == m_line->getEndPoint()) return;
    point p = FindNextPointOnObstacle(dir, lastpoint);
    lastpoint = currentPoint;
    currentPoint = p;
    obstaclePoints.push_back(p);
    assert (map[p.x][p.y] != mapSpace::obstacle);
    std::cout << p.x << "\t" << p.y << std::endl;

  } while(!currentPoint.isNeighbour(obstaclePoints[0]) or obstaclePoints.size() <= 2);
  movepath.insert(movepath.end(), obstaclePoints.begin(), obstaclePoints.end());

  //calculate the next leavepoint.
  unsigned int index = findClosestPoint(obstaclePoints, m_line->getEndPoint());
  //find if it is fastest to go back or forward
  std::cout << index << std::endl;
  if(obstaclePoints.size() / 2 + 1 > index)
  {
    //go backward
    for( unsigned int i = obstaclePoints.size() - 2; i >= index; i--)
    {
      movepath.push_back(obstaclePoints[i]);
    }
  }
  else
  {
    //go forward
    for(unsigned int i = 0; i <= index; i++)
    {
      movepath.push_back(obstaclePoints[i]);
    }
  }
  currentPoint = movepath.back();
  //std::cout << "left obstacle" << std::endl;
}
