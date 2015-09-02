#pragma once
class point
{
  public:
  int x;
  int y;
  bool operator ==(const point &p);
  bool operator !=(const point &p);
  point operator +(const point &p);
  float GetDistance(point &p);
  static float GetDistance(point p1, point p2);
  bool isNeighbour(point &p);
};
