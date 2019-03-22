#include <catch2/catch.hpp>

#include <index_sort.hpp>


TEST_CASE("sorted sequence should remain sorted", "[index_sort]")
{
    std::vector<float> arr{1.0f, 2.0f, 3.0f};
    std::vector<std::size_t> sorted_index(arr.size());

    useful::index_sort(arr.begin(), arr.end(), sorted_index.begin());

    CHECK(sorted_index[0] == 0);
    CHECK(sorted_index[1] == 1);
    CHECK(sorted_index[2] == 2);
}


TEST_CASE("reverse sorted sequence should be sorted", "[index_sort]")
{
    std::vector<float> arr{3.0f, 2.0f, 1.0f};
    std::vector<std::size_t> sorted_index(arr.size());

    useful::index_sort(arr.begin(), arr.end(), sorted_index.begin());

    CHECK(sorted_index[0] == 2);
    CHECK(sorted_index[1] == 1);
    CHECK(sorted_index[2] == 0);
}

TEST_CASE("long reverse sorted sequence should be sorted", "[index_sort]")
{
    std::vector<float> arr{7.5f, 7.0f, 5.0f, 4.0f, 3.0f, 2.0f, 1.0f};
    std::vector<std::size_t> sorted_index(arr.size());

    useful::index_sort(arr.begin(), arr.end(), sorted_index.begin());

    CHECK(sorted_index[0] == 6);
    CHECK(sorted_index[1] == 5);
    CHECK(sorted_index[2] == 4);
    CHECK(sorted_index[3] == 3);
    CHECK(sorted_index[4] == 2);
    CHECK(sorted_index[5] == 1);
    CHECK(sorted_index[6] == 0);
}
