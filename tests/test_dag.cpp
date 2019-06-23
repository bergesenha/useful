#pragma once
#include <catch2/catch.hpp>

#include <dag.hpp>


TEST_CASE("default constructed dag", "[dag]")
{
    useful::dag<char> dg;

    CHECK(dg.size() == 0);

    SECTION("insert a char")
    {
        const auto node1 = dg.insert('a');

        CHECK(dg.size() == 1);
    }
}
