#pragma once
class point
{
  public:
  int x;
  int y;
  bool operator ==(const point &p);
  bool operator !=(const point &p);
  point operator +(const point &p);
  float GetDistance(const point &p) const;
  static float GetDistance(const point p1, const point p2);
  bool isNeighbour(point &p);
};
