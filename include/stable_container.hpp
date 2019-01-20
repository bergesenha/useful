#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>


namespace useful
{

namespace detail
{
}

// deleting an element of a stable_vector doesn't affect elements before or
// after, storage is not reallocated, and the slot of the previously deleted
// element will be recycled when adding additional elements.
template <class T>
class stable_vector
{
    typedef std::vector<T> vector_type;


public:
    typedef typename vector_type::size_type size_type;

public:
    size_type
    add(const T& value)
    {
        // check for vacant position
        if(erased_.size())
        {
            const auto vacant_index = erased_.back();
            erased_.pop_back();

            new(&data_[vacant_index]) T(value);

            return vacant_index;
        }

        const auto new_index = size();
        data_.push_back(value);

        return new_index;
    }

    // access with bounds checking
    T&
    at(size_type index)
    {
        if(std::find(erased_.begin(), erased_.end(), index) != erased_.end())
        {
            throw std::out_of_range("access of deleted element");
        }

        return data_.at(index);
    }

    // access with bounds checking
    const T&
    at(size_type index) const
    {
        if(std::find(erased_.cbegin(), erased_.cend(), index) != erased_.cend())
        {
            throw std::out_of_range("access of deleted element");
        }

        return data_.at(index);
    }

    void
    erase(size_type index)
    {
        (&data_[index])->~T();
        erased_.push_back(index);
    }

    T& operator[](size_type n)
    {
        return data_[n];
    }

    const T& operator[](size_type n) const
    {
        return data_[n];
    }


    size_type
    size() const
    {
        return data_.size() - erased_.size();
    }


private:
    vector_type data_;
    std::vector<size_type> erased_;
};
} // namespace useful
