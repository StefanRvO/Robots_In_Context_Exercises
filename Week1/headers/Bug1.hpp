#include "PathFinder.hpp"

class Bug1 : public PathFinder
{
  protected:
    vector2D *cur_goal_vec = nullptr;
    virtual void findPath();
    virtual void goToNextObstacle();
    virtual void findNextLeavePoint();
  public:
    virtual ~Bug1();
    using PathFinder::PathFinder;
};
