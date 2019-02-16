#pragma once

#include <vector>
#include <utility>
#include "point_traits.hpp"


namespace useful
{
namespace multidim
{
template <class PointType>
class kdtree
{
public:
    typedef typename std::vector<PointType>::size_type size_type;
    typedef typename std::vector<PointType>::iterator unsorted_iterator;
    typedef
        typename std::vector<PointType>::const_iterator const_unsorted_iterator;

private:
    struct record
    {
        record() = default;
        record(size_type val, size_type small, size_type big)
            : value(val), smaller(small), bigger(big)
        {
        }

        size_type value;
        size_type smaller;
        size_type bigger;
    };


    void
    insert_helper(const PointType& pt, size_type level)
    {
    }

public:
    kdtree() = default;

public:
    bool
    empty() const
    {
        return dense_.empty();
    }

    size_type
    size() const
    {
        return dense_.size();
    }

    unsorted_iterator
    begin()
    {
        return dense_.begin();
    }

    unsorted_iterator
    end()
    {
        return dense_.end();
    }

    const_unsorted_iterator
    cbegin() const
    {
        return dense_.cbegin();
    }

    const_unsorted_iterator
    cend() const
    {
        return dense_.cend();
    }

    void
    insert(const PointType& pt)
    {
        if(dense_.empty())
        {
            dense_.push_back(pt);
            sparse_.emplace_back(0ul, 0ul, 0ul);
        }
    }

    void
    insert(PointType&& pt)
    {
        if(dense_.empty())
        {
            dense_.push_back(std::move(pt));
            sparse_.emplace_back(0ul, 0ul, 0ul);
        }
    }

private:
    std::vector<PointType> dense_;
    std::vector<record> sparse_;
};
} // namespace multidim
} // namespace useful
