#include "../headers/point.hpp"
#include <cmath>

bool point::operator ==(const point &p)
{
  return (this->x == p.x && this->y == p.y);
}

bool point::operator !=(const point &p)
{
  return !(this->x == p.x && this->y == p.y);
}

point point::operator +(const point &p)
{
  return {p.x + this->x, p.y + this->y};
}
float point::GetDistance(point &p)
{
  return sqrt( pow(this->x - p.x, 2) + pow(this->y - p.y, 2) );
}

float point::GetDistance(point p1, point p2)
{
  return p1.GetDistance(p2);
}

bool point::isNeighbour(point &p)
{
  for(int i = -1; i <= 1; i++)
  {
    for(int j = -1; j <=1; j++)
    {
      if(j == 0 && i == 0) continue;
      point p1 = {this->x + i, this->y + j};
      if(p1 == p) return true;
    }
  }
  return false;
}
