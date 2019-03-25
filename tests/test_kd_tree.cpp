#include <vector>
#include <catch2/catch.hpp>
#include <kd_tree.hpp>


struct point_type
{
    float x;
    float y;
};


TEST_CASE("default construct kd_tree", "[multidim::kd_tree]")
{
    useful::multidim::kd_tree<point_type> kdt;


    SECTION("insert a point")
    {
        point_type point1{1.0f, 0.0f};

        kdt.insert(point1);

        SECTION("insert other points")
        {
            point_type point2{0.5f, 1.0f};
            point_type point3{0.4f, 0.8f};
            point_type point4{2.0f, 2.0f};
            kdt.insert(point2);
            kdt.insert(point3);
            kdt.insert(point4);
        }
    }
}
