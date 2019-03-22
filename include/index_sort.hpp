#pragma once

#include <algorithm>
#include <iterator>
#include <numeric>
#include <vector>


namespace useful
{
template <class Iterator, class IndexIterator>
void
index_sort(Iterator first, Iterator last, IndexIterator index_first)
{
    typedef typename std::iterator_traits<IndexIterator>::value_type value_type;
    const auto num_elm = std::distance(first, last);
    std::iota(index_first, index_first + num_elm, 0);

    std::sort(index_first,
              index_first + num_elm,
              [first](value_type lhs, value_type rhs) {
                  return first[lhs] < first[rhs];
              });
}
} // namespace useful
