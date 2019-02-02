

#include <catch2/catch.hpp>
#include <point_traits.hpp>

struct point
{
    float x, y, z;
};

struct point2d
{
    float x, y;
};

TEST_CASE("", "")
{
    constexpr std::size_t dims = useful::point_traits<point>::dimensions;
    constexpr std::size_t dims2d = useful::point_traits<point2d>::dimensions;

    CHECK(dims == 3);
    CHECK(dims2d == 2);
}
