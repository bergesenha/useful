#pragma once

#include <vector>
#include <utility>

#include "handle_map.hpp"
#include "small_vector.hpp"


namespace useful
{
template <class T>
class tree
{
public:
    typedef T value_type;
    typedef T& reference;
    typedef const T& const_reference;
    typedef typename handle_map<T>::handle_type node_tag_type;

    typedef typename small_vector<node_tag_type,
                                  4 * sizeof(node_tag_type)>::iterator
        child_tag_iterator;

    class iterator
    {
    public:
        typedef
            typename std::iterator_traits<child_tag_iterator>::difference_type
                difference_type;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef std::random_access_iterator_tag iterator_category;

    public:
        iterator() = default;

        iterator(child_tag_iterator current, handle_map<T>& nodes_ref)
            : current_(current), nodes_ref_(&nodes_ref)
        {
        }

        reference operator*()
        {
            return nodes_ref_->operator[](*current_);
        }

        pointer operator->()
        {
            return &(*(*this));
        }

        iterator&
        operator++()
        {
            ++current_;
            return *this;
        }

        iterator
        operator++(int)
        {
            auto temp = *this;
            ++(*this);
            return temp;
        }

        iterator&
        operator--()
        {
            --current_;
            return *this;
        }

        iterator
        operator--(int)
        {
            auto temp = *this;
            --(*this);
            return temp;
        }

        difference_type
        operator-(const iterator& other) const
        {
            return current_ - other.current_;
        }

        bool
        operator==(const iterator& other) const
        {
            return current_ == other.current_;
        }


    private:
        child_tag_iterator current_;
        handle_map<T>* nodes_ref_;
    };

private:
    static constexpr inline const node_tag_type inactive_tag =
        std::numeric_limits<node_tag_type>::max();

public:
    tree() : nodes_(), parents_(), children_()
    {
        nodes_.insert(T{});
        parents_.push_back(inactive_tag);
        children_.resize(1);
    }

    tree(const T& root_node) : nodes_(), parents_(), children_()
    {
        nodes_.insert(root_node);
        parents_.push_back(inactive_tag);
        children_.resize(1);
    }

    node_tag_type
    insert_node(const T& value, node_tag_type parent)
    {
        const auto new_tag = nodes_.insert(value);

        if(new_tag >= parents_.size())
        {
            parents_.resize(new_tag + 1, inactive_tag);
            children_.resize(new_tag + 1);
        }

        parents_[new_tag] = parent;
        children_[parent].push_back(new_tag);
        return new_tag;
    }

    node_tag_type
    parent_tag(node_tag_type node)
    {
        return parents_[node];
    }

    node_tag_type
    root_tag() const
    {
        return node_tag_type{0};
    }

    std::pair<child_tag_iterator, child_tag_iterator>
    child_tags(node_tag_type node)
    {
        return std::make_pair(children_[node].begin(), children_[node].end());
    }

    std::pair<iterator, iterator>
    children(node_tag_type node)
    {
        return std::make_pair(iterator(children_[node].begin(), nodes_),
                              iterator(children_[node].end(), nodes_));
    }

    reference operator[](node_tag_type n)
    {
        return nodes_[n];
    }

    const_reference operator[](node_tag_type n) const
    {
        return nodes_[n];
    }

private:
    handle_map<T> nodes_;
    std::vector<node_tag_type> parents_;
    std::vector<small_vector<node_tag_type, 4 * sizeof(node_tag_type)>>
        children_;
};
} // namespace useful
