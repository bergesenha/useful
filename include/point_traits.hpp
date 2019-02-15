#pragma once

#include <cstddef>
#include <type_traits>
#include <iterator>
#include <algorithm>
#include <numeric>

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

    static const type&
    get(const T& pt)
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

    static const type&
    get(const T& pt)
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

    static const type&
    get(const T& pt)
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

    template <std::size_t U>
    static const value_type<U>&
    get(const PointType& pt)
    {
        return xyz_detail::helper<U, PointType>::get(pt);
    }
};

} // namespace useful


namespace useful
{

namespace point_traits_detail
{
template <class PointType,
          std::size_t N = point_traits<PointType>::dimensions - 1>
struct helper
{
    static void
    add(const PointType& lhs, const PointType& rhs, PointType& out)
    {
        point_traits<PointType>::template get<N>(out) =
            point_traits<PointType>::template get<N>(lhs) +
            point_traits<PointType>::template get<N>(rhs);

        helper<PointType, N - 1>::add(lhs, rhs, out);
    }

    template <class Arithmetic>
    static void
    divide(const PointType& lhs, Arithmetic rhs, PointType& out)
    {
        point_traits<PointType>::template get<N>(out) =
            point_traits<PointType>::template get<N>(lhs) / rhs;

        helper<PointType, N - 1>::divide(lhs, rhs, out);
    }

    template <class Function>
    static void
    apply(PointType& pt, Function f)
    {
        f(point_traits<PointType>::template get<N>(pt));

        helper<PointType, N - 1>::apply(pt, f);
    }
};


template <class PointType>
struct helper<PointType, 0>
{
    static void
    add(const PointType& lhs, const PointType& rhs, PointType& out)
    {
        point_traits<PointType>::template get<0>(out) =
            point_traits<PointType>::template get<0>(lhs) +
            point_traits<PointType>::template get<0>(rhs);
    }

    template <class Arithmetic>
    static void
    divide(const PointType& lhs, Arithmetic rhs, PointType& out)
    {
        point_traits<PointType>::template get<0>(out) =
            point_traits<PointType>::template get<0>(lhs) / rhs;
    }

    template <class Function>
    static void
    apply(PointType& pt, Function f)
    {
        f(point_traits<PointType>::template get<0>(pt));
    }
};


template <class PointType,
          std::size_t N = 0,
          std::size_t DimMax = point_traits<PointType>::dimensions - 1>
struct compare_helper
{
    static bool
    less(const PointType& lhs, const PointType& rhs, std::size_t dim)
    {
        if(N != dim)
        {
            return compare_helper<PointType, N + 1, DimMax>::less(
                lhs, rhs, dim);
        }

        return point_traits<PointType>::template get<N>(lhs) <
               point_traits<PointType>::template get<N>(rhs);
    }
};


template <class PointType, std::size_t DimMax>
struct compare_helper<PointType, DimMax, DimMax>
{
    static bool
    less(const PointType& lhs, const PointType& rhs, std::size_t dim)
    {
        return point_traits<PointType>::template get<DimMax>(lhs) <
               point_traits<PointType>::template get<DimMax>(rhs);
    }
};


} // namespace point_traits_detail


template <class PointType>
bool
less(const PointType& lhs, const PointType& rhs, std::size_t dim)
{
    return point_traits_detail::compare_helper<PointType>::less(lhs, rhs, dim);
}


template <class PointType>
PointType
add(const PointType& lhs, const PointType& rhs)
{
    PointType out;

    point_traits_detail::helper<PointType>::add(lhs, rhs, out);

    return out;
}

template <class PointType, class Arithmetic>
PointType
divide(const PointType& lhs, Arithmetic rhs)
{
    PointType out;

    point_traits_detail::helper<PointType>::divide(lhs, rhs, out);

    return out;
}

template <class Iterator>
typename std::iterator_traits<Iterator>::value_type
arithmetic_mean(Iterator first, Iterator last)
{
    const auto n = std::distance(first, last);

    typedef typename std::iterator_traits<Iterator>::value_type point_type;
    point_type out(*first++);


    out = std::accumulate(first, last, out, &add<point_type>);

    return divide(out, n);
}

template <class PointType, class Function>
void
apply(PointType& pt, Function fun)
{
    point_traits_detail::helper<PointType>::apply(pt, fun);
}


} // namespace useful
