#pragma once


#include <vector>
#include <utility>


namespace useful
{


template <class T>
class handle_map
{
public:
    typedef typename std::vector<T>::size_type size_type;
    typedef typename std::vector<size_type>::size_type handle_type;

public:
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;

public:
    handle_type
    insert(const T& value)
    {
        // always insert at end of dense storage
        dense_.push_back(value);

        handle_type new_handle;
        // find new handle
        if(free_.empty())
        {
            // no free handle slots, push back new slot
            sparse_.push_back(dense_.size() - 1);


            new_handle = sparse_.size() - 1;
        }
        else
        {
            // slot is available for new handle
            new_handle = free_.back();
            sparse_[new_handle] = dense_.size() - 1;
            free_.pop_back();
        }

        reverse_.push_back(new_handle);

        return new_handle;
    }

    template <class... Args>
    handle_type
    emplace(Args&&... args)
    {
        // always insert at end of dense storage
        dense_.emplace_back(std::forward<Args>(args)...);

        handle_type new_handle;
        // find new handle
        if(free_.empty())
        {
            // no free handle slots, push back new slot
            sparse_.push_back(dense_.size() - 1);


            new_handle = sparse_.size() - 1;
        }
        else
        {
            // slot is available for new handle
            new_handle = free_.back();
            sparse_[new_handle] = dense_.size() - 1;
            free_.pop_back();
        }

        reverse_.push_back(new_handle);

        return new_handle;
    }


    void
    erase(handle_type n)
    {
        // find handle of dense's back
        const auto index_of_dense_back = dense_.size() - 1;
        const auto back_handle = reverse_[index_of_dense_back];


        // swap element to erase with back element
        std::swap(dense_[sparse_[n]], dense_.back());
        std::swap(reverse_[n], reverse_[index_of_dense_back]);

        // update handle reference to new dense location
        sparse_[back_handle] = sparse_[n];

        // pop back
        dense_.pop_back();
        reverse_.pop_back();

        // add handle to free list
        free_.push_back(n);
    }

    size_type
    size() const
    {
        return dense_.size();
    }

    bool
    empty() const
    {
        return dense_.empty();
    }

    T& operator[](handle_type n)
    {
        return dense_[sparse_[n]];
    }

    const T& operator[](handle_type n) const
    {
        return dense_[sparse_[n]];
    }

    iterator
    begin()
    {
        return dense_.begin();
    }

    iterator
    end()
    {
        return dense_.end();
    }

    const_iterator
    cbegin() const
    {
        return dense_.cbegin();
    }

    const_iterator
    cend() const
    {
        return dense_.cend();
    }

private:
    std::vector<T> dense_;
    std::vector<size_type> sparse_;
    std::vector<handle_type> reverse_;
    std::vector<handle_type> free_;
};
} // namespace useful
