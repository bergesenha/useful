

#include <catch2/catch.hpp>
#include <point_traits.hpp>

struct point
{
    float x, y;
    int z;
};

struct point2d
{
    float x, y;
};

TEST_CASE("", "")
{
    constexpr std::size_t dims = useful::point_traits<point>::dimensions;
    constexpr std::size_t dims2d = useful::point_traits<point2d>::dimensions;

    constexpr bool x_is_float =
        std::is_same<float, useful::point_traits<point>::value_type<0>>::value;
    constexpr bool y_is_int =
        std::is_same<int, useful::point_traits<point>::value_type<2>>::value;

    CHECK(dims == 3);
    CHECK(dims2d == 2);
    CHECK(x_is_float);
    CHECK(y_is_int);
}
