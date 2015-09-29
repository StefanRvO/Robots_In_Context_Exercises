#pragma once
class point
{
  public:
  int x;
  int y;
  bool operator ==(const point &p) const;
  bool operator !=(const point &p) const;
  point operator +(const point &p) const;
  float GetDistance(const point &p) const;
  static float GetDistance(const point p1, const point p2);
  bool isNeighbour(const point &p);
};
