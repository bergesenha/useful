#pragma once

#include "point_traits.hpp"
#include "stable_container.hpp"

namespace useful
{
namespace multidim
{
template <class PointType>
class kdtree
{
    struct node;

public:
    typedef typename stable_vector<node>::size_type size_type;

private:
    struct node
    {
        PointType value;
        stable_vector<node>* storage_ref_;
        size_type smaller;
        size_type bigger;
    };

public:
    bool
    empty() const
    {
        return storage_.size() == 0;
    }

private:
    stable_vector<node> storage_;
};
} // namespace multidim
} // namespace useful
