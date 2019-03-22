#include <vector>
#include <catch2/catch.hpp>
#include <kd_tree.hpp>

struct point_type
{
    float x;
    float y;
};

static const std::vector<point_type> data{
    {1.0f, 2.0f}, {3.0f, 1.0f}, {0.0f, 4.0f}, {2.0f, 0.0f}};


using useful::multidim::kd_tree;

TEST_CASE("construct 'index' helper class", "[multidim::kd_tree_detail::index]")
{
    useful::multidim::kd_tree_detail::index<point_type, 0> index0(data.begin(),
                                                                  data.end());
    useful::multidim::kd_tree_detail::index<point_type, 1> index1(data.begin(),
                                                                  data.end());

    CHECK(index0.order[0] == 2);
    CHECK(index0.order[1] == 0);
    CHECK(index0.order[2] == 3);
    CHECK(index0.order[3] == 1);

    CHECK(index1.order[0] == 3);
    CHECK(index1.order[1] == 1);
    CHECK(index1.order[2] == 0);
    CHECK(index1.order[3] == 2);
}
