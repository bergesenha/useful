#pragma once

#include "stable_container.hpp"
#include "small_vector.hpp"


namespace useful
{
template <class T>
class dag
{
public:
    typedef typename stable_vector<T>::size_type size_type;
    typedef std::ptrdiff_t difference_type;
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;

public:
    dag() = default;

    size_type size() const;

    size_type insert(const T& node);
    void remove(size_type node_id);

    void link(size_type from, size_type to);
    void unlink(size_type from, size_type to);


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
    nodes_.erase(node_id);
    from_links_.erase(node_id);
    to_links_.erase(node_id);
}

template <class T>
typename dag<T>::size_type
dag<T>::size() const
{
    return nodes_.size();
}
} // namespace useful
