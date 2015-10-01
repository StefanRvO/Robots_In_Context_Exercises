#pragma once
#include "../headers/Map.hpp"
#include "../headers/vector2D.hpp"
#include <vector>
#include <cstdint>

class Planner
{
  protected:
    const std::vector< std::vector< mapSpace > > &map;
    long size_x;
    long size_y;
    uint64_t **wavefront = nullptr; //the wavefront map
    virtual void check_neighbour(const point &this_point, const point &neighbour, std::vector<point> &expand_points_next);
    virtual void prepare_wavefront(const point &goal);
    point get_next_point(const point &curpoint, bool *success) const;
    void setDistance(const point &p, const uint64_t value);
  public:

    Planner(const std::vector< std::vector< mapSpace > > &_map);
    virtual ~Planner();
    virtual void compute_wavefront(const point &goal); //precomputes the wavefront
    virtual uint64_t getDistance(const point &p) const;
    std::vector<point> getGoalPath(const point &start) const;

};
