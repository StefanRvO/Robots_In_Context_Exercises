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
  if(cur_goal_vec != nullptr) delete cur_goal_vec;
}

std::vector<point> PathFinder::getPath(point startpos, point endpos)
{
  movepath.clear();
  movepath.push_back(startpos);
  goal = endpos;
  if(cur_goal_vec != nullptr) delete cur_goal_vec;
  if(m_line != nullptr) delete m_line;
  m_line = new vector2D(startpos, endpos);
  cur_goal_vec = new vector2D(startpos, endpos);
  currentPoint = startpos;
  while(currentPoint != goal)
  {
    goToNextObstacle();
    FindClosestPointOnObstacle();
    delete cur_goal_vec;
    cur_goal_vec = new vector2D(currentPoint, endpos);

  }
  return movepath;
}

void PathFinder::FindClosestPointOnObstacle()
{
  std::cout << "Hit obstacle" << std::endl;
  direction dir = static_cast<direction>(rand()%2);
  std::vector <point> obstaclePoints;

  //go all around the obstacle
  obstaclePoints.push_back(currentPoint);
  do
  {
    if(currentPoint == goal) return;
    point p = FindNextPointOnObstacle(dir);
    currentPoint = p;
    std::cout << p.x << "\t" << p.y << std::endl;
    obstaclePoints.push_back(p);
    assert ((*map)[p.x][p.y] != mapSpace::obstacle);

  } while(!currentPoint.isNeighbour(obstaclePoints[0]) or obstaclePoints.size() <= 2);
  movepath.insert(movepath.end(), obstaclePoints.begin(), obstaclePoints.end());

  //calculate the next leavepoint.
  unsigned int index = findNextLeavePoint(obstaclePoints);
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
  std::cout << "left obstacle" << std::endl;

}

int PathFinder::findNextLeavePoint(std::vector <point> &obstaclePoints)
{
  int i = 0;
  float mindistance = std::numeric_limits<float>::infinity();
  int bestPoint = 0;
  for(auto &p : obstaclePoints)
  {
    float distance = p.GetDistance(goal);
    if(distance < mindistance)
    {
      bestPoint = i;
      mindistance = distance;
    }
    i++;
  }
  return bestPoint;
}
void PathFinder::goToNextObstacle()
{
  bool hitObstacle = false;
  while(!hitObstacle)
  {
    point p = FindNextPointOnLine(&hitObstacle);
    currentPoint = p;
    //std::cout << p.x << "\t" << p.y << std::endl;
    movepath.push_back(p);
    //std::cout << p.x << "\t" << p.x << std::endl;
    //std::cout << point::GetDistance(p, goal) << std::endl;
    if(currentPoint == goal) return;
  }
  movepath.pop_back();
  currentPoint = movepath.back();

}
point PathFinder::FindNextPointOnLine(bool *obstacle)
{
  //check all 8 directions collect the ones which are closer to the goal
  //than the current.

  point closestpoint = {0, 0};
  std::vector <point> candidatepoints;
  float curdistance = currentPoint.GetDistance(goal);
  for(int i = -1; i <= 1; i++)
  {
    for(int j = -1; j <=1; j++)
    {
      if(j == 0 && i == 0) continue;
      point this_point = {currentPoint.x + i, currentPoint.y + j};
      float distance = this_point.GetDistance(goal);
      if(distance <= curdistance) candidatepoints.push_back(this_point);
    }
  }
  //select the wanted point as the one closest to the straight line to goal.
  float mindistance = std::numeric_limits<float>::infinity();
  for (auto &p : candidatepoints)
  {
    float distance = cur_goal_vec->distanceToPoint(p);
    if (distance < mindistance)
    {
      closestpoint = p;
      mindistance = distance;
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
