#include <vector>
#include <catch2/catch.hpp>
#include <kd_tree.hpp>

struct point_type
{
    float x;
    float y;
};

static const std::vector<point_type> data{
    {0.0f, 0.0f}, {1.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}};


using useful::multidim::kd_tree;


TEST_CASE("default construct a kd_tree", "[multidim::kd_tree]")
{
    kd_tree<point_type> kdt(data.cbegin(), data.cend());

    (void)kdt;
}
