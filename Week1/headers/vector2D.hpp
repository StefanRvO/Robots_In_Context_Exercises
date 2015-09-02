//class for working with 2Dvectors
#pragma once
#include <cmath>
#include "point.hpp"
#include <cstdlib>

class vector2D
{
  private:
    point point_1;
    point point_2;
  public:
    point getStartPoint();
    point getEndPoint();
    vector2D(point startpoint, point endpoint);
    float GetLenght();
    float distanceToPoint(point p);

};
