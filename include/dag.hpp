#pragma once

#include <cstddef>
#include <utility>
#include <iterator>
#include <algorithm>

#include "stable_container.hpp"
#include "small_vector.hpp"


namespace useful
{
template <class T>
class dag;


template <class T>
class link_iterator
{
public:
    typedef typename stable_vector<T>::size_type size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;

public:
    link_iterator() = default;
    link_iterator(dag<T>& dg, small_vector<size_type>& links, size_type index);
    reference operator*();
    link_iterator& operator++();
    link_iterator operator++(int);
    difference_type operator-(const link_iterator& other) const;

private:
    dag<T>* ref_;
    small_vector<size_type>* link_ref_;
    size_type index_;
};

template <class T>
class dag
{
public:
    typedef typename stable_vector<T>::size_type size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;

    class link_iterator
    {
    public:
        typedef std::ptrdiff_t difference_type;

    public:
        link_iterator() = default;

        link_iterator(dag& dg, small_vector<size_type>& links, size_type index)
            : ref_(&dg), link_ref_(&links), index_(index)
        {
        }

        reference operator*()
        {
            const auto node_index = link_ref_->operator[](index_);
            return ref_->nodes_[node_index];
        }

        link_iterator&
        operator++()
        {
            ++index_;
            return *this;
        }

        link_iterator
        operator++(int)
        {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        difference_type
        operator-(const link_iterator& other) const
        {
            return index_ - other.index_;
        }


    private:
        dag* ref_;
        small_vector<size_type>* link_ref_;
        size_type index_;
    };

public:
    dag() = default;

    size_type size() const;

    size_type insert(const T& node);
    void remove(size_type node_id);

    void link(size_type from, size_type to);
    void unlink(size_type from, size_type to);

    std::pair<link_iterator, link_iterator> children(size_type node_id);
    std::pair<link_iterator, link_iterator> parents(size_type node_id);


private:
    stable_vector<T> nodes_;
    stable_vector<small_vector<size_type>> from_links_;
    stable_vector<small_vector<size_type>> to_links_;
};

} // namespace useful


namespace useful
{
template <class T>
typename dag<T>::size_type
dag<T>::insert(const T& node)
{
    from_links_.push_back(small_vector<size_type>());
    to_links_.push_back(small_vector<size_type>());
    return nodes_.push_back(node);
}

template <class T>
void
dag<T>::link(typename dag<T>::size_type from, typename dag<T>::size_type to)
{
    from_links_[from].push_back(to);
    to_links_[to].push_back(from);
}

template <class T>
void
dag<T>::unlink(typename dag<T>::size_type from, typename dag<T>::size_type to)
{
    auto from_it = &from_links_[from][0];
    auto found_from =
        std::find(from_it, from_it + from_links_[from].size(), to);
    from_links_[from].erase(found_from);

    auto to_it = &to_links_[to][0];
    auto found_to = std::find(to_it, to_it + to_links_[to].size(), from);
    to_links_[to].erase(found_to);
}

template <class T>
void
dag<T>::remove(typename dag<T>::size_type node_id)
{
    std::for_each(from_links_[node_id].begin(),
                  from_links_[node_id].end(),
                  [this, node_id](size_type parent_id) {
                      auto found = std::find(to_links_[parent_id].begin(),
                                             to_links_[parent_id].end(),
                                             node_id);
                      to_links_[parent_id].erase(found);
                  });
    std::for_each(to_links_[node_id].begin(),
                  to_links_[node_id].end(),
                  [this, node_id](size_type child_id) {
                      auto found = std::find(from_links_[child_id].begin(),
                                             from_links_[child_id].end(),
                                             node_id);
                      from_links_[child_id].erase(found);
                  });

    from_links_.erase(node_id);
    to_links_.erase(node_id);
    nodes_.erase(node_id);
}

template <class T>
typename dag<T>::size_type
dag<T>::size() const
{
    return nodes_.size();
}


template <class T>
std::pair<typename dag<T>::link_iterator, typename dag<T>::link_iterator>
dag<T>::children(size_type node_id)
{
    return std::make_pair(link_iterator(*this, from_links_[node_id], 0),
                          link_iterator(*this,
                                        from_links_[node_id],
                                        from_links_[node_id].size()));
}


template <class T>
std::pair<typename dag<T>::link_iterator, typename dag<T>::link_iterator>
dag<T>::parents(size_type node_id)
{
    return std::make_pair(
        link_iterator(*this, to_links_[node_id], 0),
        link_iterator(*this, to_links_[node_id], to_links_[node_id].size()));
}

template <class T>
link_iterator<T>::link_iterator(dag<T>& dg,
                                small_vector<size_type>& links,
                                size_type index)
    : ref_(&dg), link_ref_(&links), index_(index)
{
}

template <class T>
typename link_iterator<T>::reference link_iterator<T>::operator*()
{
    const auto node_index = link_ref_->operator[](index_);
    return ref_->nodes_[node_index];
}

template <class T>
link_iterator<T>&
link_iterator<T>::operator++()
{
    ++index_;
    return *this;
}

template <class T>
link_iterator<T>
link_iterator<T>::operator++(int)
{
    auto temp = *this;
    ++(*this);
    return temp;
}

template <class T>
typename link_iterator<T>::difference_type
link_iterator<T>::operator-(const link_iterator& other) const
{
    return index_ - other.index_;
}
} // namespace useful

namespace std
{
template <class T>
struct iterator_traits<useful::link_iterator<T>>
{
    typedef typename useful::link_iterator<T>::difference_type difference_type;
    typedef typename useful::link_iterator<T>::value_type value_type;
    typedef typename useful::link_iterator<T>::pointer pointer;
    typedef typename useful::link_iterator<T>::reference reference;
    typedef std::bidirectional_iterator_tag iterator_category;
};
} // namespace std
