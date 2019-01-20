#pragma once

#include <vector>
#include <iterator>
#include <functional>

namespace useful
{


template <class PropertyType>
class persistent_iterator_;

template <class PropertyType>
class ordered_iterator_
{
public:
    typedef typename std::vector<PropertyType>::size_type size_type;
    typedef typename std::vector<size_type>::difference_type difference_type;
    typedef PropertyType value_type;
    typedef PropertyType* pointer;
    typedef PropertyType& reference;

    typedef typename std::vector<size_type>::size_type index_type;

    friend class persistent_iterator_<PropertyType>;

public:
    ordered_iterator_(index_type current_index_to_index,
                      const std::vector<size_type>& indices,
                      std::vector<PropertyType>& nodes)
        : current_index_to_index_(current_index_to_index),
          indices_(indices),
          nodes_ref_(nodes)
    {
    }

public:
    // Iterator
    reference operator*()
    {
        return nodes_ref_.get()[indices_[current_index_to_index_]];
    }

    ordered_iterator_&
    operator++()
    {
        ++current_index_to_index_;
        return *this;
    }

    // Equality comparable
    bool
    operator==(const ordered_iterator_& other) const
    {
        return (current_index_to_index_ == other.current_index_to_index_);
    }

    // InputIterator
    bool
    operator!=(const ordered_iterator_& other) const
    {
        return !(*this == other);
    }

    pointer operator->()
    {
        return &(nodes_ref_.get()[indices_[current_index_to_index_]]);
    }

    ordered_iterator_
    operator++(int)
    {
        auto temp = *this;
        ++(*this);
        return temp;
    }

    // BidirectionalIterator
    ordered_iterator_&
    operator--()
    {
        --current_index_to_index_;
        return *this;
    }

    ordered_iterator_
    operator--(int)
    {
        auto temp = *this;
        --(*this);
        return temp;
    }

    // RandomAccessIterator
    ordered_iterator_&
    operator+=(difference_type n)
    {
        current_index_to_index_ += n;
        return *this;
    }

    ordered_iterator_
    operator+(difference_type n) const
    {
        return ordered_iterator_(
            current_index_to_index_ + n, indices_, nodes_ref_);
    }

    friend ordered_iterator_
    operator+(difference_type lhs, const ordered_iterator_& rhs)
    {
        return rhs + lhs;
    }

    ordered_iterator_&
    operator-=(difference_type n)
    {
        current_index_to_index_ -= n;
        return *this;
    }

    ordered_iterator_
    operator-(difference_type n) const
    {
        return ordered_iterator_(
            current_index_to_index_ - n, indices_, nodes_ref_);
    }

    difference_type
    operator-(const ordered_iterator_& other) const
    {
        return current_index_to_index_ - other.current_index_to_index_;
    }

    reference operator[](difference_type n)
    {
        return nodes_ref_.get()[indices_[current_index_to_index_ + n]];
    }

    bool
    operator<(const ordered_iterator_& other) const
    {
        return current_index_to_index_ < other.current_index_to_index_;
    }

    bool
    operator>(const ordered_iterator_& other) const
    {
        return current_index_to_index_ > other.current_index_to_index_;
    }

    bool
    operator<=(const ordered_iterator_& other) const
    {
        return current_index_to_index_ <= other.current_index_to_index_;
    }

    bool
    operator>=(const ordered_iterator_& other) const
    {
        return current_index_to_index_ >= other.current_index_to_index_;
    }


private:
    index_type current_index_to_index_;
    std::vector<size_type> indices_;
    std::reference_wrapper<std::vector<PropertyType>> nodes_ref_;
};


template <class PropertyType>
class persistent_iterator_
{
    template <class NodeType, class EdgeType>
    friend class dag;

public:
    typedef typename std::vector<PropertyType>::size_type size_type;

    typedef typename std::vector<PropertyType>::difference_type difference_type;
    typedef PropertyType value_type;
    typedef PropertyType* pointer;
    typedef PropertyType& reference;

public:
    persistent_iterator_(size_type current_index,
                         std::vector<PropertyType>& nodes)
        : current_index_(current_index), nodes_ref_(nodes)
    {
    }

    persistent_iterator_(const ordered_iterator_<PropertyType>& other)
        : current_index_(other.indices_[other.current_index_to_index_]),
          nodes_ref_(other.nodes_ref_)
    {
    }

public:
    // Iterator concept member functions
    reference operator*()
    {
        return nodes_ref_.get()[current_index_];
    }

    persistent_iterator_&
    operator++()
    {
        ++current_index_;
        return *this;
    }

    // EqualityComparable
    bool
    operator==(const persistent_iterator_& other) const
    {
        return (current_index_ == other.current_index_) &&
               (&(nodes_ref_.get()) == &(other.nodes_ref_.get()));
    }

    // InputIterator
    bool
    operator!=(const persistent_iterator_& other) const
    {
        return !(*this == other);
    }

    pointer operator->()
    {
        return &(nodes_ref_.get()[current_index_]);
    }

    persistent_iterator_
    operator++(int)
    {
        auto temp = *this;
        ++(*this);

        return temp;
    }

    // BidirectionalIterator
    persistent_iterator_&
    operator--()
    {
        --current_index_;
        return *this;
    }

    persistent_iterator_
    operator--(int)
    {
        auto temp = *this;
        --(*this);
        return temp;
    }

    // RandomAccessIterator
    persistent_iterator_&
    operator+=(difference_type n)
    {
        current_index_ += n;
        return *this;
    }

    persistent_iterator_
    operator+(difference_type n) const
    {
        return persistent_iterator_(current_index_ + n, nodes_ref_);
    }

    friend persistent_iterator_
    operator+(difference_type lhs, const persistent_iterator_& rhs)
    {
        return rhs + lhs;
    }

    persistent_iterator_&
    operator-=(difference_type n)
    {
        current_index_ -= n;
        return *this;
    }

    persistent_iterator_
    operator-(difference_type n) const
    {
        return persistent_iterator_(current_index_ - n, nodes_ref_);
    }

    difference_type
    operator-(const persistent_iterator_& other) const
    {
        return current_index_ - other.current_index_;
    }

    reference operator[](difference_type n)
    {
        return nodes_ref_.get()[current_index_ + n];
    }

    bool
    operator<(const persistent_iterator_& other) const
    {
        return current_index_ < other.current_index_;
    }

    bool
    operator>(const persistent_iterator_& other) const
    {
        return current_index_ > other.current_index_;
    }

    bool
    operator<=(const persistent_iterator_& other) const
    {
        return current_index_ <= other.current_index_;
    }

    bool
    operator>=(const persistent_iterator_& other) const
    {
        return current_index_ >= other.current_index_;
    }

private:
    size_type current_index_;
    std::reference_wrapper<std::vector<PropertyType>> nodes_ref_;
};
} // namespace useful


namespace std
{
template <class PropertyType>
struct iterator_traits<useful::persistent_iterator_<PropertyType>>
{
    typedef typename useful::persistent_iterator_<PropertyType>::difference_type
        difference_type;

    typedef typename useful::persistent_iterator_<PropertyType>::value_type
        value_type;

    typedef
        typename useful::persistent_iterator_<PropertyType>::pointer pointer;

    typedef typename useful::persistent_iterator_<PropertyType>::reference
        reference;

    typedef random_access_iterator_tag iterator_category;
};


template <class PropertyType>
struct iterator_traits<useful::ordered_iterator_<PropertyType>>
{
    typedef typename useful::persistent_iterator_<PropertyType>::difference_type
        difference_type;

    typedef typename useful::persistent_iterator_<PropertyType>::value_type
        value_type;

    typedef
        typename useful::persistent_iterator_<PropertyType>::pointer pointer;

    typedef typename useful::persistent_iterator_<PropertyType>::reference
        reference;

    typedef random_access_iterator_tag iterator_category;
};
} // namespace std

