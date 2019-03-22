#pragma once

#include <vector>


namespace useful
{
namespace multidim
{
template <class PointType>
class kdtree
{
    struct record;

public:
    typedef typename std::vector<record>::size_type size_type;

private:
    struct record
    {
        PointType value;
        size_type parent;
        size_type smaller;
        size_type bigger;
    };

public:
private:
    std::vector<record> storage_;
};
} // namespace multidim
} // namespace useful
