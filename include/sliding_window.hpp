#pragma once

#include <cstddef>
#include <memory>
#include <functional>
#include <ratio>
#include <iterator>
#include <initializer_list>
#include <limits>

#include <circular_iterator.hpp>


namespace useful
{
template <class T,
          std::size_t Size,
          template <class, class, std::size_t>
          class StoragePolicy>
class sliding_window
    : public StoragePolicy<sliding_window<T, Size, StoragePolicy>, T, Size>
{
    typedef StoragePolicy<sliding_window<T, Size, StoragePolicy>, T, Size>
        storage_policy;

public:
    static const std::size_t size = Size;

    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    typedef circular_iterator<T*> iterator;
    typedef circular_iterator<const T*> const_iterator;

public:
    sliding_window()
        : storage_policy(),
          head_(storage_policy::begin(),
                storage_policy::end(),
                storage_policy::begin())
    {
    }

    template <class Iterator>
    sliding_window(Iterator first, Iterator last) : sliding_window()
    {
        for(; first != last; ++first, ++head_)
        {
            *head_ = *first;
        }
    }

    sliding_window(std::initializer_list<T> il)
        : sliding_window(il.begin(), il.end())
    {
    }

    sliding_window(const sliding_window& other)
        : storage_policy(other),
          head_(storage_policy::begin(),
                storage_policy::end(),
                storage_policy::begin() + other.head_.underlying_position())
    {
    }

    sliding_window&
    operator=(const sliding_window& other)
    {
        std::copy(other.cbegin(), other.cend(), begin());
        return *this;
    }

public:
    void
    push_back(const T& value)
    {
        *head_ = value;
        ++head_;
    }

    void
    push_back(const T& value, size_type n)
    {
        std::fill_n(head_, n, value);
        head_ += n;
    }

    void
    push_front(const T& value)
    {
        --head_;
        *head_ = value;
    }

    void
    push_front(const T& value, size_type n)
    {
        std::fill_n(std::make_reverse_iterator(head_), n, value);
        head_ -= n;
    }

    reference
    front()
    {
        return *head_;
    }

    const_reference
    front() const
    {
        return *head_;
    }

    reference
    back()
    {
        return *(head_ + Size - 1);
    }

    const_reference
    back() const
    {
        return *(head_ + Size - 1);
    }

    reference operator[](size_type n)
    {
        return head_[n];
    }

    const_reference operator[](size_type n) const
    {
        return head_[n];
    }

    iterator
    begin()
    {
        return head_;
    }

    iterator
    end()
    {
        return head_ + Size;
    }

    const_iterator
    cbegin() const
    {
        return head_;
    }

    const_iterator
    cend() const
    {
        return head_ + Size;
    }


    bool
    operator==(const sliding_window& other) const
    {
        return std::equal(cbegin(), cend(), other.cbegin());
    }

    bool
    operator!=(const sliding_window& other) const
    {
        return !operator==(other);
    }

private:
    iterator head_;
};

template <class Derived, class T, std::size_t Size>
class stack_storage
{
public:
    stack_storage() : buffer_{}
    {
    }

    stack_storage(const stack_storage& other)
    {
        std::copy(std::begin(other.buffer_),
                  std::end(other.buffer_),
                  std::begin(buffer_));
    }

    stack_storage&
    operator=(const stack_storage& other)
    {
        std::copy(std::begin(other.buffer_),
                  std::end(other.buffer_),
                  std::begin(buffer_));

        return *this;
    }

    void
    swap(stack_storage& other)
    {
        std::swap(buffer_, other.buffer_);
    }

public:
    T*
    begin()
    {
        return buffer_;
    }

    T*
    end()
    {
        return buffer_ + Size;
    }

protected:
    T buffer_[Size];
};

template <class Derived, class T, std::size_t Size>
class static_heap_storage
{
public:
    static_heap_storage() : buffer_(new T[Size]{})
    {
    }

    static_heap_storage(const static_heap_storage& other)
        : static_heap_storage()
    {
        std::copy(
            other.buffer_.get(), other.buffer_.get() + Size, buffer_.get());
    }

    static_heap_storage(static_heap_storage&&) = default;

    static_heap_storage&
    operator=(const static_heap_storage& other)
    {
        auto temp = other;
        swap(temp);
        return *this;
    }

    static_heap_storage& operator=(static_heap_storage&&) = default;

    void
    swap(static_heap_storage& other)
    {
        std::swap(buffer_, other.buffer_);
    }

public:
    T*
    begin()
    {
        return buffer_.get();
    }

    T*
    end()
    {
        return buffer_.get() + Size;
    }

protected:
    std::unique_ptr<T[]> buffer_;
};


template <class T, std::size_t Size>
using stack_sliding_window = sliding_window<T, Size, stack_storage>;

template <class T, std::size_t Size>
using static_heap_sliding_window = sliding_window<T, Size, static_heap_storage>;


namespace detail
{
struct runtime_ratio
{
    runtime_ratio() : num(1), den(1)
    {
    }

    template <std::intmax_t Num, std::intmax_t Denom>
    constexpr runtime_ratio(std::ratio<Num, Denom>) : num(Num), den(Denom)
    {
    }

    std::intmax_t num;
    std::intmax_t den;
};

template <class Numeric>
Numeric operator*(const runtime_ratio& lhs, Numeric rhs)
{
    return rhs * static_cast<Numeric>(lhs.num) / static_cast<Numeric>(lhs.den);
}

template <class Numeric>
Numeric operator*(Numeric lhs, const runtime_ratio& rhs)
{
    return rhs * lhs;
}


template <class Numeric>
Numeric
operator/(Numeric lhs, const runtime_ratio& rhs)
{
    return lhs * static_cast<Numeric>(rhs.den) / static_cast<Numeric>(rhs.num);
}

inline bool
operator==(const runtime_ratio& lhs, const runtime_ratio& rhs)
{
    return lhs.num == rhs.num && lhs.den == rhs.den;
}

inline bool
operator!=(const runtime_ratio& lhs, const runtime_ratio& rhs)
{
    return lhs.num != rhs.num || lhs.den != rhs.den;
}
} // namespace detail


template <class KeyType,
          class ValueType,
          std::size_t Size,
          class Compare = std::less<KeyType>,
          class SlidingWindowType = static_heap_sliding_window<ValueType, Size>>
class sliding_window_map
{
public:
    typedef ValueType value_type;
    typedef ValueType& reference;
    typedef const ValueType& const_reference;
    typedef typename SlidingWindowType::iterator iterator;
    typedef typename SlidingWindowType::const_iterator const_iterator;
    typedef typename SlidingWindowType::difference_type difference_type;
    typedef typename SlidingWindowType::size_type size_type;

public:
    sliding_window_map() : sliding_buffer_(), precision_(), origin_()
    {
    }


    template <std::intmax_t Num, std::intmax_t Denom>
    sliding_window_map(std::ratio<Num, Denom>)
        : sliding_buffer_(), precision_(std::ratio<Num, Denom>()), origin_()
    {
    }


    sliding_window_map(KeyType origin)
        : sliding_buffer_(), precision_(), origin_(origin / precision_)
    {
    }


    template <std::intmax_t Num, std::intmax_t Denom>
    sliding_window_map(KeyType origin, std::ratio<Num, Denom>)
        : sliding_buffer_(),
          precision_(std::ratio<Num, Denom>()),
          origin_(origin / precision_)
    {
    }


    std::pair<KeyType, KeyType>
    window() const
    {
        return std::make_pair(static_cast<KeyType>(origin_) * precision_,
                              static_cast<KeyType>(origin_ + Size) *
                                  precision_);
    }


    ValueType&
    at(KeyType k)
    {
        const auto index = index_of_key(k);
        const Compare c;
        if(c(index, 0) || (!c(index, Size)))
        {
            throw std::out_of_range("Key outside of current window");
        }

        return sliding_buffer_[index];
    }


    ValueType& operator[](KeyType k)
    {
        return sliding_buffer_[k / precision_ - origin_];
    }


    void
    insert_or_assign(KeyType k, const ValueType& v)
    {
        const auto index = index_of_key(k);
        const Compare c;

        if(c(index, 0))
        {
            // below current window
            sliding_buffer_.push_front(ValueType(), -index);
            sliding_buffer_.front() = v;
            origin_ += index * precision_;
        }
        else if(!c(index, Size))
        {
            // above current window
            sliding_buffer_.push_back(ValueType(), index - Size + 1);
            sliding_buffer_.back() = v;
            origin_ += (index - Size + 1) * precision_;
        }
        else
        {
            // within current window
            sliding_buffer_[index] = v;
        }
    }

    iterator
    begin()
    {
        return sliding_buffer_.begin();
    }

    iterator
    end()
    {
        return sliding_buffer_.end();
    }

    const_iterator
    cbegin() const
    {
        return sliding_buffer_.cbegin();
    }

    const_iterator
    cend() const
    {
        return sliding_buffer_.cend();
    }

    bool
    operator==(const sliding_window_map& other) const
    {
        if(origin_ == other.origin_)
        {
            if(precision_ == other.precision_)
            {
                return sliding_buffer_ == other.sliding_buffer_;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }

    bool
    operator!=(const sliding_window_map& other) const
    {
        return !operator==(other);
    }

    constexpr size_type
    size() const
    {
        return Size;
    }

    constexpr size_type
    max_size() const
    {
        return std::numeric_limits<size_type>::max;
    }

    constexpr bool
    empty() const
    {
        return false;
    }

    void
    swap(sliding_window_map& other)
    {
        using std::swap;

        swap(sliding_buffer_, other.sliding_buffer_);
        swap(precision_, other.precision_);
        swap(origin_, other.origin_);
    }

    KeyType
    get_key(const_iterator it)
    {
        const auto index = it - cbegin();

        return static_cast<KeyType>(origin_ + index) * precision_;
    }

private:
    difference_type
    index_of_key(KeyType k)
    {
        return (k - origin_ * precision_) / precision_;
    }

private:
    SlidingWindowType sliding_buffer_;
    detail::runtime_ratio precision_;
    difference_type origin_;
};
} // namespace useful


namespace std
{
template <class Derived, class T, std::size_t Size>
void
swap(useful::stack_storage<Derived, T, Size>& lhs,
     useful::stack_storage<Derived, T, Size>& rhs)
{
    lhs.swap(rhs);
}

template <class Derived, class T, std::size_t Size>
void
swap(useful::static_heap_storage<Derived, T, Size>& lhs,
     useful::static_heap_storage<Derived, T, Size>& rhs)
{
    lhs.swap(rhs);
}
} // namespace std
