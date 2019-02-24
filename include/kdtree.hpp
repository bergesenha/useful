#pragma once

#include <vector>
#include <utility>
#include "point_traits.hpp"


namespace useful
{
namespace multidim
{
template <class PointType>
class kdtree
{
public:
    typedef typename std::vector<PointType>::size_type size_type;
    typedef typename std::vector<PointType>::iterator unsorted_iterator;
    typedef
        typename std::vector<PointType>::const_iterator const_unsorted_iterator;

private:
    struct record
    {
        record() = default;
        record(size_type small, size_type big, size_type par);

        size_type smaller;
        size_type bigger;
        size_type parent;
    };


    void insert_helper(const PointType& pt, size_type index, size_type level);

    void insert_helper(PointType&& pt, size_type index, size_type level);


public:
    class depth_iterator
    {
        enum class state
        {
            unvisited,
            smaller_visited,
            visited
        };

    public:
        depth_iterator() = default;

        depth_iterator(kdtree* ref, size_type current)
            : depth_stack_{state::unvisited}, ref_(ref), current_(current)
        {
        }

        depth_iterator(const std::vector<state>& depth_stack,
                       kdtree* ref,
                       size_type current)
            : depth_stack_(depth_stack), ref_(ref), current_(current)
        {
        }

        PointType& operator*()
        {
            return ref_->dense_[current_];
        }

        PointType* operator->()
        {
            return &(ref_->dense_[current_]);
        }

        depth_iterator&
        operator++()
        {
            if(depth_stack_.empty())
            {
                return *this;
            }
            else
            {
                const auto node_state = depth_stack_.back();
                if(node_state == state::unvisited)
                {
                    if(smaller())
                    {
                        depth_stack_.back() = state::smaller_visited;
                        current_ = smaller();
                        depth_stack_.push_back(state::unvisited);

                        return *this;
                    }
                    else
                    {
                        depth_stack_.back() = state::smaller_visited;
                        return operator++();
                    }
                }
                else if(node_state == state::smaller_visited)
                {
                    if(bigger())
                    {
                        depth_stack_.back() = state::visited;
                        depth_stack_.push_back(state::unvisited);
                        current_ = bigger();

                        return *this;
                    }
                    else
                    {
                        depth_stack_.pop_back();
                        current_ = parent();
                        return operator++();
                    }
                }
                else if(node_state == state::visited)
                {
                    depth_stack_.pop_back();
                    current_ = parent();

                    return operator++();
                }
            }

            return *this;
        }

        bool
        operator==(const depth_iterator& other) const
        {
            return current_ == other.current_ &&
                   depth_stack_.empty() == other.depth_stack_.empty();
        }

        bool
        operator!=(const depth_iterator& other) const
        {
            return current_ != other.current_ ||
                   depth_stack_.empty() != other.depth_stack_.empty();
        }

    private:
        size_type
        smaller() const
        {
            return ref_->sparse_[current_].smaller;
        }
        size_type
        bigger() const
        {
            return ref_->sparse_[current_].bigger;
        }

        size_type
        parent() const
        {
            return ref_->sparse_[current_].parent;
        }

    private:
        std::vector<state> depth_stack_;
        kdtree* ref_;
        size_type current_;
    };

public:
    kdtree() = default;

    bool
    empty() const
    {
        return dense_.empty();
    }

    size_type
    size() const
    {
        return dense_.size();
    }

    unsorted_iterator
    begin()
    {
        return dense_.begin();
    }

    unsorted_iterator
    end()
    {
        return dense_.end();
    }

    const_unsorted_iterator
    cbegin() const
    {
        return dense_.cbegin();
    }

    const_unsorted_iterator
    cend() const
    {
        return dense_.cend();
    }

    void
    insert(const PointType& pt)
    {
        if(dense_.empty())
        {
            dense_.push_back(pt);
            sparse_.emplace_back(0ul, 0ul, 0ul);
        }
        else
        {
            insert_helper(pt, 0ul, 0ul);
        }
    }

    void
    insert(PointType&& pt)
    {
        if(dense_.empty())
        {
            dense_.push_back(std::move(pt));
            sparse_.emplace_back(0ul, 0ul, 0ul);
        }
        else
        {
            insert_helper(std::move(pt), 0ul, 0ul);
        }
    }

    depth_iterator
    depth_begin()
    {
        if(sparse_.empty())
        {
            return depth_iterator({}, this, 0ul);
        }
        else
        {
            return depth_iterator(this, 0ul);
        }
    }

    depth_iterator
    depth_end()
    {
        return depth_iterator({}, this, 0ul);
    }

private:
    std::vector<PointType> dense_;
    std::vector<record> sparse_;
};
} // namespace multidim
} // namespace useful


namespace useful
{
namespace multidim
{
template <class PointType>
kdtree<PointType>::record::record(size_type small, size_type big, size_type par)
    : smaller(small), bigger(big), parent(par)
{
}

template <class PointType>
void
kdtree<PointType>::insert_helper(const PointType& pt,
                                 size_type index,
                                 size_type level)
{
    if(level >= point_traits<PointType>::dimensions)
    {
        level = 0ul;
    }
    else
    {
        ++level;
    }

    const record& current = sparse_[index];

    if(less(pt, dense_[index], level))
    {
        if(current.smaller)
        {
            // recurse
            insert_helper(pt, current.smaller, level);
        }
        else
        {
            dense_.push_back(pt);
            const size_type new_index = dense_.size() - 1;
            sparse_[index].smaller = new_index;
            sparse_.emplace_back(0ul, 0ul, index);
        }
    }
    else
    {
        if(current.bigger)
        {
            insert_helper(pt, current.bigger, level);
        }
        else
        {
            dense_.push_back(pt);
            const size_type new_index = dense_.size() - 1;
            sparse_[index].bigger = new_index;
            sparse_.emplace_back(0ul, 0ul, index);
        }
    }
}

template <class PointType>
void
kdtree<PointType>::insert_helper(PointType&& pt,
                                 size_type index,
                                 size_type level)
{
    if(level >= point_traits<PointType>::dimensions)
    {
        level = 0ul;
    }
    else
    {
        ++level;
    }

    const record& current = sparse_[index];

    if(less(pt, dense_[index], level))
    {
        if(current.smaller)
        {
            // recurse
            insert_helper(std::move(pt), current.smaller, level);
        }
        else
        {
            dense_.push_back(std::move(pt));
            const size_type new_index = dense_.size() - 1;
            sparse_[index].smaller = new_index;
            sparse_.emplace_back(0ul, 0ul, index);
        }
    }
    else
    {
        if(current.bigger)
        {
            insert_helper(std::move(pt), current.bigger, level);
        }
        else
        {
            dense_.push_back(std::move(pt));
            const size_type new_index = dense_.size() - 1;
            sparse_[index].bigger = new_index;
            sparse_.emplace_back(0ul, 0ul, index);
        }
    }
}
} // namespace multidim
} // namespace useful
