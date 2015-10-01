#include "../headers/Planner.hpp"
#include <iostream>
#include <limits>
#include <cassert>

static point neighbours[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {-1, -1}, {1, 1}, {1, -1}, {-1, 1}}; //the neighbours a cell have

Planner::Planner(const std::vector< std::vector< mapSpace > > &_map)
:map(_map)
{
  size_x = map.size();
  size_y = map[0].size();
}
Planner::~Planner()
{
  //deallocate the wavefront map
  if(wavefront != nullptr)
  {
    for(long i = 0; i < size_x; i++)
    {
      delete[] wavefront[i];
    }
    delete[] wavefront;
  }
}

void Planner::compute_wavefront(const point &goal)
{
  prepare_wavefront(goal);
  auto expand_points_this = new std::vector<point>; //points to expand from in this run
  auto expand_points_next = new std::vector<point>; //points to expand from in the next run
  expand_points_this->push_back(goal);  //start expansion from goal.
  while(expand_points_this->size())
  {
    for( auto this_point : *expand_points_this) //run through all the points
    {
      for(uint8_t i = 0; i < sizeof(neighbours) / sizeof(neighbours[i]); i++)
        check_neighbour(this_point, this_point + neighbours[i], *expand_points_next);
    }
    //clear list of points to check, and switch to next list.
    expand_points_this->clear();
    std::swap(expand_points_this, expand_points_next);
  }
  delete expand_points_this;
  delete expand_points_next;
}

inline void Planner::check_neighbour(const point &this_point, const point &neighbour, std::vector<point> &expand_points_next)
{ //check if we should expand in to the given neighbour, and if so, add it
  //to the list of points to check on next run.

  //First check if we are out of bounds
  if(neighbour.y < 0 || neighbour.y > size_y - 1 ||
      neighbour.x < 0 || neighbour.x > size_x - 1)
  {
    return;
  }
  //only expand into point if value is either 0 or at least 2 larger than current value.
  if(getDistance(neighbour) == 0 ||
    getDistance(neighbour) > getDistance(this_point) + 1)
  {
    //set point to current value + 1
    setDistance(neighbour, getDistance(this_point) + 1);
    expand_points_next.push_back(neighbour);
  }
}

inline void Planner::prepare_wavefront(const point &goal)
{ //prepare wavefront structure for computations
  //First allocate the required space
  if(wavefront == nullptr)
  {
    wavefront = new uint64_t *[size_x];
    for(long i = 0; i < size_x; i++)
    {
      wavefront[i] = new uint64_t[size_y];
    }
  }
  //fill with initial values according to map (obstacles become 1, freespace 0)
  for(int x = 0; x < size_x; x++)
  {
    for(int y = 0; y < size_y; y++)
    {
      setDistance({x, y}, (uint64_t)map[x][y]);
    }
  }
  setDistance(goal, 2); //set goal
}
inline uint64_t Planner::getDistance(const point &p) const
{ //returns distance to goal
  return wavefront[p.x][p.y];
}
inline void Planner::setDistance(const point &p, const uint64_t value)
{
  wavefront[p.x][p.y] = value;
}

std::vector<point> Planner::getGoalPath(const point &start) const
{
  std::vector<point> path;
  path.push_back(start);
  while(getDistance(path.back()) != 1 && getDistance(path.back()) != 2) //continue until we are either in an obstacle or at the goal
  {
    bool success;
    point next_point = get_next_point(path.back(), &success);
    if(success) path.push_back(next_point);
    else break;
  }
  return path;
}

inline point Planner::get_next_point(const point &curpoint, bool *success) const
{
  for(uint8_t i = 0; i < sizeof(neighbours) / sizeof(neighbours[i]); i++) //go through all possible neighbours
  {
    //Check if we are out of bounds
    auto neighbour = curpoint + neighbours[i];
    if(neighbour.y < 0 || neighbour.y > size_y - 1 ||
        neighbour.x < 0 || neighbour.x > size_x - 1)
    {
      continue;
    }
    if(getDistance(neighbour) < getDistance(curpoint) && getDistance(neighbour) != 1)
    {
      *success = true;
      return neighbour;
    }
  }
  //if we didn't find a point, we are probably in an obstacle
  //set success to false and return current point
  *success = false;
  return curpoint;
}
