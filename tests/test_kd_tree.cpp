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

TEST_CASE("construct 'index' helper class", "[multidim::kd_tree_detail::index]")
{
    useful::multidim::kd_tree_detail::index<point_type, 0> index0(data.begin(),
                                                                  data.end());
}
