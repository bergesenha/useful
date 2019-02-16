
#include <iostream>
#include <chrono>
#include <random>

#include "kdtree.hpp"

struct space_point
{
    float x, y, z;
};


using useful::multidim::kdtree;

int
main()
{
    kdtree<space_point> kdt;

    std::random_device rd;

    const std::size_t num_samples = 10000000;

    std::uniform_real_distribution<float> urd;
    std::vector<space_point> points;
    points.reserve(num_samples);

    for(std::size_t i = 0; i < num_samples; ++i)
    {
        points.push_back(space_point{urd(rd), urd(rd), urd(rd)});
    }

    for(std::size_t i = 0; i < points.size(); ++i)
    {
        kdt.insert(points[i]);
    }
}
