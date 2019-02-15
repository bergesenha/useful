#include <vector>

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

TEST_CASE("test point_traits with xyz-like struct", "[point_traits]")
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

TEST_CASE("test point_traits with array-type point", "[point_traits]")
{
    float pt1[3] = {4.0f, 5.0f, 6.0f};

    constexpr std::size_t dims =
        useful::point_traits<decltype(pt1)>::dimensions;

    float elm1 = useful::point_traits<decltype(pt1)>::get<0>(pt1);

    CHECK(dims == 3);
    CHECK(elm1 == Approx(4.0f));
}

TEST_CASE("test point_traits with xyz-like point", "[point_traits]")
{
    point pt1{1.0f, 2.0f, 3};
    const point pt2{4.0f, 5.0f, 60};

    const auto xval = useful::point_traits<point>::get<0>(pt1);
    const auto yval = useful::point_traits<point>::get<2>(pt1);

    const auto xval2 = useful::point_traits<point>::get<0>(pt2);
    const auto yval2 = useful::point_traits<point>::get<2>(pt2);

    point pt3 = useful::add(pt1, pt2);

    bool pt1_2_less_pt2_2 = useful::less(pt1, pt2, 2);


    CHECK(xval == Approx(1.0f));
    CHECK(yval == 3);
    CHECK(pt3.x == Approx(5.0f));
    CHECK(pt3.y == Approx(7.0f));
    CHECK(pt3.z == Approx(63));
    CHECK(pt1_2_less_pt2_2);

    SECTION("apply a function to each dimension")
    {
        useful::apply(pt1, [](auto& elm) { elm *= 2; });

        CHECK(pt1.x == Approx(2.0f));
        CHECK(pt1.y == Approx(4.0f));
        CHECK(pt1.z == 6);
    }
}


TEST_CASE("test point algorithms with a range of points", "[point_traits]")
{
    std::vector<point> pvec{{1.0f, 2.0f, 3}, {2.0f, 1.0f, 2}, {3.0f, 3.0f, 1}};

    auto res = useful::arithmetic_mean(pvec.begin(), pvec.end());

    CHECK(res.x == Approx(2.0f));
}
