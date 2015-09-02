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
    point getStartPoint()
    {
      return point_1;
    }
    point getEndPoint()
    {
      return point_2;
    }
    vector2D(point startpoint, point endpoint)
    {
      point_1 = startpoint;
      point_2 = endpoint;
    }

    float GetLenght()
    {
      return sqrt( pow(point_1.x - point_2.x, 2) + pow(point_1.y - point_2.y, 2));
    }

    float distanceToPoint(point p)
    {
      float num = (point_2.y - point_1.y) * p.x - (point_2.x - point_1.x) * p.y;
      num += point_2.x * point_1.y - point_2.y * point_1.x;
      num = fabs(num);
      float den = sqrt(pow(point_2.y - point_1.y, 2) + pow(point_2.x - point_1.x, 2));
      return num / den;
    }

};
