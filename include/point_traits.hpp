#pragma once

#include <cstddef>
#include <type_traits>

namespace useful
{

template <class PointType, class, class, class>
struct point_traits_;


template <class PointType>
using point_traits = point_traits_<PointType, void, void, void>;


template <class T, std::size_t N>
struct point_traits_<T[N], void, void, void>
{
    static const std::size_t dimensions = N;

    template <std::size_t U>
    using value_type = T;

    template <std::size_t U>
    static value_type<U>& get(T (&pt)[N])
    {
        return pt[U];
    }
};


namespace xyz_detail
{
template <std::size_t, class T>
struct helper;

template <class T>
struct helper<0, T>
{
    typedef decltype(std::declval<T>().x) type;

    static type&
    get(T& pt)
    {
        return pt.x;
    }
};

template <class T>
struct helper<1, T>
{
    typedef decltype(std::declval<T>().y) type;

    static type&
    get(T& pt)
    {
        return pt.y;
    }
};

template <class T>
struct helper<2, T>
{
    typedef decltype(std::declval<T>().z) type;

    static type&
    get(T& pt)
    {
        return pt.z;
    }
};
}; // namespace xyz_detail


template <class PointType, class Y, class Z>
struct point_traits_<PointType,
                     std::void_t<decltype(std::declval<PointType>().x)>,
                     Y,
                     Z>
{
    static const std::size_t dimensions = 1;

    template <std::size_t U>
    using value_type = typename xyz_detail::helper<U, PointType>::type;

    template <std::size_t U>
    static value_type<U>&
    get(PointType& pt)
    {
        return xyz_detail::helper<U, PointType>::get(pt);
    }
};


template <class PointType, class Z>
struct point_traits_<PointType,
                     std::void_t<decltype(std::declval<PointType>().x)>,
                     std::void_t<decltype(std::declval<PointType>().y)>,
                     Z>
{
    static const std::size_t dimensions = 2;

    template <std::size_t U>
    using value_type = typename xyz_detail::helper<U, PointType>::type;

    template <std::size_t U>
    static value_type<U>&
    get(PointType& pt)
    {
        return xyz_detail::helper<U, PointType>::get(pt);
    }
};


template <class PointType>
struct point_traits_<PointType,
                     std::void_t<decltype(std::declval<PointType>().x)>,
                     std::void_t<decltype(std::declval<PointType>().y)>,
                     std::void_t<decltype(std::declval<PointType>().z)>>
{
    static const std::size_t dimensions = 3;

    template <std::size_t U>
    using value_type = typename xyz_detail::helper<U, PointType>::type;


    template <std::size_t U>
    static value_type<U>&
    get(PointType& pt)
    {
        return xyz_detail::helper<U, PointType>::get(pt);
    }
};

} // namespace useful
