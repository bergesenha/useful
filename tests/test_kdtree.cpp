#include <catch2/catch.hpp>
#include <kdtree.hpp>


struct point_type
{
    float x, y;
};

using useful::multidim::kdtree;

TEST_CASE("default construct a kdtree", "[multidim::kdtree]")
{
    kdtree<point_type> kdt;

    CHECK(kdt.empty());
    CHECK(kdt.size() == 0);

    CHECK(kdt.depth_begin() == kdt.depth_end());

    SECTION("insert first point")
    {
        kdt.insert(point_type{0.0f, 0.0f});

        CHECK(kdt.empty() == false);
        CHECK(kdt.size() == 1);

        CHECK(kdt.depth_begin() != kdt.depth_end());
        CHECK(kdt.depth_begin()->x == Approx(0.0f));
        CHECK(kdt.depth_begin()->y == Approx(0.0f));

        SECTION("increment iterator to first")
        {
            auto it = kdt.depth_begin();
            ++it;

            CHECK(it == kdt.depth_end());
        }

        SECTION("insert another point")
        {
            kdt.insert(point_type{1.0f, -1.0f});

            CHECK(kdt.depth_begin() != kdt.depth_end());
            CHECK(kdt.depth_begin()->x == Approx(0.0f));
            CHECK(kdt.depth_begin()->y == Approx(0.0f));
            CHECK(kdt.size() == 2);

            SECTION("increment iterator to first")
            {
                auto it = kdt.depth_begin();
                ++it;

                CHECK(it->x == Approx(1.0f));
                CHECK(it->y == Approx(-1.0f));

                SECTION("increment past end")
                {
                    ++it;

                    CHECK(it == kdt.depth_end());
                }
            }
        }
    }
}
