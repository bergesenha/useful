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

public:
    dag() = default;

    size_type insert(const T& node);

    void link(size_type from, size_type to);

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
} // namespace useful
