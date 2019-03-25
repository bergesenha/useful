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
        std::for_each(first, last, [this](const PointType& pt) { insert(pt); });
    }


    void
    insert(const PointType& pt)
    {
        if(storage_.empty())
        {
            storage_.emplace_back(pt, 0ul, 0ul, 0ul);
            return;
        }

        // start at root node
        size_type current = 0ul;
        size_type level = 0ul;
        bool point_inserted = false;

        while(!point_inserted)
        {
            // cycle back to first dimension again
            if(level == point_traits<PointType>::dimensions)
            {
                level = 0ul;
            }

            // compare point to current node
            if(less(pt, storage_[current].value, level)) // smaller
            {
                if(storage_[current].smaller != 0ul) // has smaller child
                {
                    current = storage_[current].smaller;
                }
                else // leaf node found
                {
                    storage_.emplace_back(pt, current, 0ul, 0ul);
                    storage_[current].smaller = storage_.size() - 1;
                    point_inserted = true;
                }
            }
            else // bigger or equal
            {
                if(storage_[current].bigger != 0ul)
                {
                    current = storage_[current].bigger;
                }
                else
                {
                    storage_.emplace_back(pt, current, 0ul, 0ul);
                    storage_[current].bigger = storage_.size() - 1;
                    point_inserted = true;
                }
            }

            ++level;
        }
    }

private:
    std::vector<record> storage_;
};
} // namespace multidim
} // namespace useful
