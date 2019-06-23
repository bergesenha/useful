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

        SECTION("insert another char")
        {
            const auto node2 = dg.insert('b');

            CHECK(dg.size() == 2);

            SECTION("link 2 nodes")
            {
                dg.link(node1, node2);
            }
        }
    }
}
