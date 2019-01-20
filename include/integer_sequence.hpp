#pragma once

#include <utility>


namespace useful
{
namespace int_seq
{
////////////////////////////////////////////////////////////////////////////////
// get first integer of a std::integer_sequence as value
template <class IntSeq>
struct head;

template <class IntType, IntType First, IntType... Rest>
struct head<std::integer_sequence<IntType, First, Rest...>>
{
    static const IntType value = First;
    typedef IntType value_type;
};

template <class IntSeq>
constexpr typename head<IntSeq>::value_type head_v = head<IntSeq>::value;


////////////////////////////////////////////////////////////////////////////////
// get tail of a std::integer_sequence
template <class IntSeq>
struct tail;

template <class IntType, IntType First, IntType... Rest>
struct tail<std::integer_sequence<IntType, First, Rest...>>
{
    typedef std::integer_sequence<IntType, Rest...> type;
};

template <class IntSeq>
using tail_t = typename tail<IntSeq>::type;


////////////////////////////////////////////////////////////////////////////////
// append an integer to the end of a std::integer_sequence
template <class IntType, class IntSeq, IntType Num>
struct append;

template <class IntType, IntType... Nums, IntType Num>
struct append<IntType, std::integer_sequence<IntType, Nums...>, Num>
{
    typedef std::integer_sequence<IntType, Nums..., Num> type;
};

template <class IntSeq, typename IntSeq::value_type Num>
using append_t =
    typename append<typename IntSeq::value_type, IntSeq, Num>::type;


////////////////////////////////////////////////////////////////////////////////
// append an integer if it satisfies UnaryPredicate<IntType N>
template <class IntSeq,
          typename IntSeq::value_type N,
          template <typename IntSeq::value_type>
          class UnaryPredicate,
          bool Satisfies = UnaryPredicate<N>::value>
struct append_if
{
    typedef IntSeq type;
};

template <class IntSeq,
          typename IntSeq::value_type N,
          template <typename IntSeq::value_type>
          class UnaryPredicate>
struct append_if<IntSeq, N, UnaryPredicate, true>
{
    typedef append_t<IntSeq, N> type;
};

template <class IntSeq,
          typename IntSeq::value_type N,
          template <typename IntSeq::value_type>
          class UnaryPredicate>
using append_if_t = typename append_if<IntSeq, N, UnaryPredicate>::type;


////////////////////////////////////////////////////////////////////////////////
// generate an integer sequence from a range [Begin, End)
template <class IntType,
          IntType Begin,
          IntType End,
          class Accum = std::integer_sequence<IntType>>
struct integer_sequence_from_range
    : integer_sequence_from_range<IntType,
                                  Begin + 1,
                                  End,
                                  append_t<Accum, Begin>>
{
};

template <class IntType, IntType End, class Accum>
struct integer_sequence_from_range<IntType, End, End, Accum>
{
    typedef Accum type;
};

template <class IntType, IntType Begin, IntType End>
using integer_sequence_from_range_t =
    typename integer_sequence_from_range<IntType, Begin, End>::type;


////////////////////////////////////////////////////////////////////////////////
// return std::integer_sequence of integers in IntSeq that satisfy
// UnaryPredicate, ie UnaryPredicate<Num>::value == true
template <class IntSeq,
          template <typename IntSeq::value_type>
          class UnaryPredicate,
          class Accum = std::integer_sequence<typename IntSeq::value_type>>
struct filter : filter<tail_t<IntSeq>,
                       UnaryPredicate,
                       append_if_t<Accum, head_v<IntSeq>, UnaryPredicate>>
{
};

template <class IntType, template <IntType> class UnaryPredicate, class Accum>
struct filter<std::integer_sequence<IntType>, UnaryPredicate, Accum>
{
    typedef Accum type;
};

template <class IntSeq,
          template <typename IntSeq::value_type>
          class UnaryPredicate>
using filter_t = typename filter<IntSeq, UnaryPredicate>::type;


////////////////////////////////////////////////////////////////////////////////
// return an array with values from std::integer_sequence
template <class IntSeq>
struct integer_sequence_to_array;

template <class IntType, IntType... Seq>
struct integer_sequence_to_array<std::integer_sequence<IntType, Seq...>>
{
    typedef IntType value_type;
    static constexpr IntType value[] = {Seq...};
};

template <class IntType>
struct integer_sequence_to_array<std::integer_sequence<IntType>>
{
    typedef IntType value_type;
    static constexpr IntType* value = nullptr;
};

template <class IntType, IntType... Seq>
constexpr IntType
    integer_sequence_to_array<std::integer_sequence<IntType, Seq...>>::value[];


template <class IntSeq>
struct length;

template <class IntType, IntType... Seq>
struct length<std::integer_sequence<IntType, Seq...>>
{
    static const std::size_t value = sizeof...(Seq);
};

template <class IntSeq>
constexpr std::size_t length_v = length<IntSeq>::value;
} // namespace int_seq
} // namespace useful
