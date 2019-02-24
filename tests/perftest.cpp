
#include <iostream>
#include <chrono>
#include <random>

#include "kdtree.hpp"

struct space_point
{
    float x, y, z;
};


using useful::multidim::kdtree;

std::ostream&
operator<<(std::ostream& out, const space_point& pt)
{

    return out;
}

int
main()
{
    kdtree<space_point> kdt;

    kdt.insert(space_point{3.0f, 2.0f, 3.0f});
    kdt.insert(space_point{4.0f, 3.0f, 3.0f});
    kdt.insert(space_point{2.0f, 1.0f, 3.0f});
    kdt.insert(space_point{4.0f, 1.0f, 3.0f});
}
