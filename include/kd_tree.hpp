#pragma once

#include <vector>
#include <algorithm>
#include "point_traits.hpp"
#include "index_sort.hpp"


namespace useful
{
namespace multidim
{

namespace kd_tree_detail
{

template <class PointType, std::size_t Dim>
struct index
{
    template <class RandomAccessIterator>
    index(RandomAccessIterator first, RandomAccessIterator last)
        : order(std::distance(first, last))
    {
        index_sort(first,
                   last,
                   order.begin(),
                   [](const PointType& lhs, const PointType& rhs) {
                       return point_traits<PointType>::template get<Dim>(lhs) <
                              point_traits<PointType>::template get<Dim>(rhs);
                   });
    }

    std::vector<std::size_t> order;
};

template <class PointType, class IndexSequence>
struct unwrap;

template <class PointType, std::size_t... Sequence>
struct unwrap<PointType, std::index_sequence<Sequence...>>
    : index<PointType, Sequence>...
{
    template <class RandomAccessIterator>
    unwrap(RandomAccessIterator first, RandomAccessIterator last)
        : index<PointType, Sequence>(first, last)...
    {
    }
};

template <class PointType>
struct super_index
    : unwrap<PointType,
             std::make_index_sequence<point_traits<PointType>::dimensions>>
{
    template <class RandomAccessIterator>
    super_index(RandomAccessIterator first, RandomAccessIterator last)
        : unwrap<PointType,
                 std::make_index_sequence<point_traits<PointType>::dimensions>>(
              first, last)
    {
    }
};

} // namespace kd_tree_detail

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
        kd_tree_detail::super_index<PointType> s_index(first, last);
    }

private:
    std::vector<record> storage_;
};
} // namespace multidim
} // namespace useful
