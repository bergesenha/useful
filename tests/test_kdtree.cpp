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

    REQUIRE(kdt.empty());
    REQUIRE(kdt.size() == 0);
    CHECK(kdt.depth_begin() == kdt.depth_end());

    SECTION("insert first point")
    {
        kdt.insert(point_type{0.0f, 0.0f});

        CHECK(kdt.empty() == false);
        CHECK(kdt.size() == 1);
        CHECK(kdt.depth_begin() != kdt.depth_end());

        SECTION("insert another point")
        {
            kdt.insert(point_type{1.0f, -1.0f});

            CHECK(kdt.size() == 2);
            CHECK(kdt.depth_begin() != kdt.depth_end());

            SECTION("get iterator to beginning of tree")
            {
                auto iter = kdt.depth_begin();

                CHECK(iter->x == Approx(0.0f));
                CHECK(iter->y == Approx(0.0f));

                SECTION("increment iterator")
                {
                    ++iter;

                    CHECK(iter->x == Approx(1.0f));
                    CHECK(iter->y == Approx(-1.0f));

                    SECTION("increment past end")
                    {
                        ++iter;

                        CHECK(iter == kdt.depth_end());
                    }
                }
            }
        }

        SECTION("get beginning of depth iteration")
        {
            auto iter = kdt.depth_begin();

            CHECK(iter->x == Approx(0.0f));
            CHECK(iter->y == Approx(0.0f));

            SECTION("increment past end")
            {
                ++iter;

                CHECK(iter == kdt.depth_end());
            }
        }
    }
}
