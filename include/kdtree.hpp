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
    typedef PointType value_type;

private:
    struct record
    {
        record() = default;
        record(size_type small, size_type big, size_type par)
            : smaller(small), bigger(big), parent(par)
        {
        }

        size_type smaller;
        size_type bigger;
        size_type parent;
    };


    void
    insert_helper(const PointType& pt, size_type index, size_type level)
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
                // insert leaf
                const auto smaller_index = dense_.size();
                parallel_push_back(pt, 0ul, 0ul, current.smaller);
                sparse_[index].smaller = smaller_index;
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
                const auto bigger_index = dense_.size();
                parallel_push_back(pt, 0ul, 0ul, current.bigger);
                sparse_[index].bigger = bigger_index;
            }
        }
    }

    void
    insert_helper(PointType&& pt, size_type index, size_type level)
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
                // insert leaf
                const auto smaller_index = dense_.size();
                parallel_push_back(std::move(pt), 0ul, 0ul, current.smaller);
                sparse_[index].smaller = smaller_index;
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
                const auto bigger_index = dense_.size();
                parallel_push_back(std::move(pt), 0ul, 0ul, current.bigger);
                sparse_[index].bigger = bigger_index;
            }
        }
    }

    template <class... RecordArgs>
    void
    parallel_push_back(const PointType& pt, RecordArgs&&... rec_args)
    {
        dense_.push_back(pt);
        sparse_.emplace_back(std::forward<RecordArgs>(rec_args)...);
    }

    template <class... RecordArgs>
    void
    parallel_push_back(PointType&& pt, RecordArgs&&... rec_args)
    {
        dense_.push_back(std::move(pt));
        sparse_.emplace_back(std::forward<RecordArgs>(rec_args)...);
    }

public:
    class depth_iterator
    {
        enum class state
        {
            none_visited,
            smaller_visited,
            bigger_visited
        };

    public:
        typedef value_type& reference;
        typedef value_type* pointer;

    public:
        depth_iterator() = default;

        depth_iterator(size_type curr, kdtree* ref)
            : visit_stack_(), ref_(ref), current_(curr)
        {
        }

        reference operator*()
        {
            return ref_->dense_[current_];
        }

        pointer operator->()
        {
            return &(ref_->dense_[current_]);
        }

        depth_iterator&
        operator++()
        {
            const record& rec = ref_->sparse_[current_];

            if(visit_stack_.back() == state::smaller_visited)
            {
                // down bigger branch
                visit_stack_.back() = state::bigger_visited;
                visit_stack_.push_back(state::none_visited);
                current_ = rec.bigger;

                return *this;
            }

            if(visit_stack_.back() == state::bigger_visited)
            {
                // backtrack
                current_ = rec.parent;
                visit_stack_.pop_back();

                while(visit_stack_.back() == state::bigger_visited &&
                      visit_stack_.empty() != true)
                {
                    current_ = ref_->sparse_[current_].parent;
                    visit_stack_.pop_back();
                }

                if(visit_stack_.empty())
                {
                    return *this;
                }

                return ++(*this);
            }

            // down smaller branch
            visit_stack_.back() = state::smaller_visited;
            visit_stack_.push_back(state::none_visited);
            current_ = rec.smaller;

            return *this;
        }

    private:
        std::vector<state> visit_stack_;
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

    depth_iterator
    depth_begin()
    {
        return depth_iterator(0ul, this);
    }

    void
    insert(const PointType& pt)
    {
        if(dense_.empty())
        {
            parallel_push_back(pt, 0ul, 0ul, 0ul);
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
            parallel_push_back(std::move(pt), 0ul, 0ul, 0ul);
        }
        else
        {
            insert_helper(std::move(pt), 0ul, 0ul);
        }
    }


private:
    std::vector<PointType> dense_;
    std::vector<record> sparse_;
};
} // namespace multidim
} // namespace useful
