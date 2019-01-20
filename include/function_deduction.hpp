#pragma once

#include "type_list.hpp"

namespace useful
{
////////////////////////////////////////////////////////////////////////////////
// returns return type of a function type of function pointer type
template <class FunctionType>
struct deduce_return_type;

template <class RetType, class... ArgTypes>
struct deduce_return_type<RetType(ArgTypes...)>
{
    typedef RetType type;
};

template <class RetType, class... ArgTypes>
struct deduce_return_type<RetType (*)(ArgTypes...)>
{
    typedef RetType type;
};

template <class RetType, class ObjectType, class... ArgTypes>
struct deduce_return_type<RetType (ObjectType::*)(ArgTypes...)>
{
    typedef RetType type;
};

template <class RetType, class ObjectType, class... ArgTypes>
struct deduce_return_type<RetType (ObjectType::*)(ArgTypes...) const>
{
    typedef RetType type;
};

template <class FunctionType>
using deduce_return_type_t = typename deduce_return_type<FunctionType>::type;


////////////////////////////////////////////////////////////////////////////////
// returns parameter types in a type_list
template <class FunctionType>
struct deduce_parameter_types;

template <class RetType, class... ArgTypes>
struct deduce_parameter_types<RetType(ArgTypes...)>
{
    typedef t_list::type_list<ArgTypes...> type;
};

template <class RetType, class... ArgTypes>
struct deduce_parameter_types<RetType (*)(ArgTypes...)>
{
    typedef t_list::type_list<ArgTypes...> type;
};

template <class RetType, class ObjectType, class... ArgTypes>
struct deduce_parameter_types<RetType (ObjectType::*)(ArgTypes...)>
{
    typedef t_list::type_list<ArgTypes...> type;
};

template <class RetType, class ObjectType, class... ArgTypes>
struct deduce_parameter_types<RetType (ObjectType::*)(ArgTypes...) const>
{
    typedef t_list::type_list<ArgTypes...> type;
};

template <class FunctionType>
using deduce_parameter_types_t =
    typename deduce_parameter_types<FunctionType>::type;


////////////////////////////////////////////////////////////////////////////////
// returns type of object the member function is a member of

template <class FunctionType>
struct deduce_object_type;

template <class RetType, class ObjectType, class... ArgTypes>
struct deduce_object_type<RetType (ObjectType::*)(ArgTypes...)>
{
    typedef ObjectType type;
};

template <class RetType, class ObjectType, class... ArgTypes>
struct deduce_object_type<RetType (ObjectType::*)(ArgTypes...) const>
{
    typedef ObjectType type;
};

template <class FunctionType>
using deduce_object_type_t = typename deduce_object_type<FunctionType>::type;
} // namespace useful
