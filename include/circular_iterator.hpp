#pragma once

#include <type_traits>
#include <iterator>
#include <utility>


namespace useful
{
namespace detail
{
template <class Iterator, class IteratorTag>
constexpr bool is_iterator_of_category_v =
    std::is_same<typename std::iterator_traits<Iterator>::iterator_category,
                 IteratorTag>::value;


template <class Iterator, class IteratorTag>
constexpr bool is_at_least_iterator_of_category_v = std::is_base_of<
    IteratorTag,
    typename std::iterator_traits<Iterator>::iterator_category>::value;
} // namespace detail


/** \class circular_iterator circular_iterator.hpp <circular_iterator.hpp>
 *
 * \brief An iterator adaptor that adapts any iterator to wrap around when
 * incremented beyond a range determined on construction
 */
template <class UnderlyingIterator>
class circular_iterator
{
    // friend declarations primary purose to allow private member access when
    // converting to const_iterator version of UnderlyingIterator
    template <class OtherUnderlying>
    friend class circular_iterator;

public:
    static_assert(
        detail::is_at_least_iterator_of_category_v<
            UnderlyingIterator,
            std::random_access_iterator_tag>,
        "circular_iterator requires random access underlying iterator");

    typedef typename std::iterator_traits<UnderlyingIterator>::value_type
        value_type;
    typedef typename std::iterator_traits<UnderlyingIterator>::difference_type
        difference_type;
    typedef
        typename std::iterator_traits<UnderlyingIterator>::reference reference;
    typedef typename std::iterator_traits<UnderlyingIterator>::pointer pointer;
    typedef std::random_access_iterator_tag iterator_category;

public:
    circular_iterator() = default;

    circular_iterator(UnderlyingIterator first,
                      UnderlyingIterator last,
                      UnderlyingIterator current)
        : wrap_size_(std::distance(first, last)),
          index_(std::distance(first, current)),
          base_(first)
    {
    }

    // converting constructor primary purpose for conversion to const_iterator
    // underlying
    template <class OtherUnderlying>
    circular_iterator(const circular_iterator<OtherUnderlying>& other)
        : wrap_size_(other.wrap_size_), index_(other.index_), base_(other.base_)
    {
    }

    bool
    operator==(const circular_iterator& other)
    {
        const auto base_diff = base_ - other.base_;
        return base_diff == other.index_ - index_;
    }

    bool
    operator!=(const circular_iterator& other)
    {
        return !operator==(other);
    }

    reference operator*()
    {
        return base_[local_index(index_)];
    }

    circular_iterator&
    operator++()
    {
        ++index_;
        return *this;
    }

    circular_iterator
    operator++(int)
    {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    circular_iterator&
    operator--()
    {
        --index_;
        return *this;
    }

    circular_iterator
    operator--(int)
    {
        auto temp = *this;
        --(*this);
        return temp;
    }


    circular_iterator&
    operator+=(difference_type n)
    {
        index_ += n;
        return *this;
    }

    circular_iterator&
    operator-=(difference_type n)
    {
        index_ -= n;
        return *this;
    }

    friend circular_iterator
    operator+(const circular_iterator& lhs, difference_type rhs)
    {
        auto temp = lhs;
        temp += rhs;
        return temp;
    }

    friend circular_iterator
    operator+(difference_type lhs, const circular_iterator& rhs)
    {
        return rhs + lhs;
    }

    friend circular_iterator
    operator-(const circular_iterator& lhs, difference_type rhs)
    {
        auto temp = lhs;
        temp -= rhs;
        return temp;
    }

    difference_type
    operator-(const circular_iterator& other)
    {
        return index_ - other.index_;
    }

    typename circular_iterator::reference
    operator[](typename circular_iterator::difference_type n)
    {
        return base_[local_index(index_ + n)];
    }

    bool
    operator<(const circular_iterator& other)
    {
        return index_ < other.index_;
    }

    bool
    operator>(const circular_iterator& other)
    {
        return index_ > other.index_;
    }

    bool
    operator<=(const circular_iterator& other)
    {
        return index_ <= other.index_;
    }

    bool
    operator>=(const circular_iterator& other)
    {
        return index_ >= other.index_;
    }

public:
    difference_type
    underlying_position() const
    {
        return index_;
    }

    difference_type
    wrap_size() const
    {
        return wrap_size_;
    }

private:
    difference_type
    local_index(difference_type index) const
    {
        const auto remainder = index % wrap_size_;
        if(remainder < 0)
        {
            return wrap_size_ + remainder;
        }
        else
        {
            return remainder;
        }
    }

private:
    difference_type wrap_size_;
    difference_type index_;
    UnderlyingIterator base_;
};
} // namespace useful
