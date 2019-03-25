#pragma once

#include <vector>
#include <algorithm>
#include "point_traits.hpp"
#include "index_sort.hpp"


namespace useful
{
namespace multidim
{

template <class PointType>
class kd_tree
{
    struct record;

public:
    typedef typename std::vector<record>::size_type size_type;

private:
    struct record
    {
        record(PointType val, size_type par, size_type small, size_type big)
            : value(val), parent(par), smaller(small), bigger(big)
        {
        }
        PointType value;
        size_type parent;
        size_type smaller;
        size_type bigger;
    };

public:
    kd_tree() = default;

    template <class Iterator>
    kd_tree(Iterator first, Iterator last) : storage_()
    {
    }


    void
    insert(const PointType& pt)
    {
        size_type current_index = 0ul;
        size_type level = 0ul;
    }

private:
    std::vector<record> storage_;
};
} // namespace multidim
} // namespace useful
