#pragma once

#include "PathFinder.hpp"

class Bug2 : public PathFinder
{
  protected:
    vector2D *cur_goal_vec = nullptr;
    virtual void findPath();
    virtual void goToNextObstacle();
    virtual void findNextLeavePoint();
    point prev_leave_point ={0, 0};
  public:
    virtual ~Bug2();
    using PathFinder::PathFinder;
};
