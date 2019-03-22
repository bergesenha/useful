#pragma once

#include <vector>
#include <algorithm>
#include "point_traits.hpp"


namespace useful
{
namespace multidim
{
namespace super_index_detail
{
template <std::size_t Dim>
struct order
{
};

template <class IndexSequence>
struct holder;


template <std::size_t... Sequence>
struct holder<std::index_sequence<Sequence...>> : order<Sequence>...
{
};

} // namespace super_index_detail

template <class PointType>
class super_index
    : super_index_detail::holder<
          std::make_index_sequence<point_traits<PointType>::dimensions>>
{
public:
    template <class Iterator>
    super_index(Iterator first, Iterator last)
    {
    }

private:
    template <std::size_t Dim>
    void
    sort_on_dimension()
    {
        super_index_detail::order<Dim>* value_holder =
            static_cast<super_index_detail::order<Dim>*>(this);
    }

private:
};


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
        if(first != last)
        {
        }
    }

private:
    std::vector<record> storage_;
};
} // namespace multidim
} // namespace useful
