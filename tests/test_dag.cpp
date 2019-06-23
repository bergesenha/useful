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

                SECTION("children of node1 should be node2")
                {
                    auto children_ts = dg.children(node1);
                    CHECK(*children_ts.first == 'b');
                }

                SECTION("parent of node2 should be node1")
                {
                    auto parent_its = dg.parents(node2);
                    CHECK(*parent_its.first == 'a');
                }
            }
        }
    }
}
