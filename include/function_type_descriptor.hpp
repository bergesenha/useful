#pragma once

#include <cstddef>
#include "type_list.hpp"


namespace useful
{
namespace ft_descriptor
{


////////////////////////////////////////////////////////////////////////////////
// holds function type information at compile time
template <class ReturnType, bool Constness, class... ParamTypes>
struct function_type_descriptor
{
};


////////////////////////////////////////////////////////////////////////////////
// get return type of function_type_descriptor
template <class FuncTypeDesc>
struct return_type;

template <class ReturnType, bool Constness, class... ParamTypes>
struct return_type<
    function_type_descriptor<ReturnType, Constness, ParamTypes...>>
{
    typedef ReturnType type;
};

template <class FuncTypeDesc>
using return_type_t = typename return_type<FuncTypeDesc>::type;


////////////////////////////////////////////////////////////////////////////////
// get parameter types as type_list
template <class FuncTypeDesc>
struct parameter_types;


template <class ReturnType, bool Constness, class... ParamTypes>
struct parameter_types<
    function_type_descriptor<ReturnType, Constness, ParamTypes...>>
{
    typedef t_list::type_list<ParamTypes...> type;
};

template <class FuncTypeDesc>
using parameter_types_t = typename parameter_types<FuncTypeDesc>::type;


////////////////////////////////////////////////////////////////////////////////
// check if function type is const
template <class FuncTypeDesc>
struct is_const;


template <class ReturnType, bool Constness, class... ParamTypes>
struct is_const<function_type_descriptor<ReturnType, Constness, ParamTypes...>>
{
    static const bool value = Constness;
};

template <class FuncTypeDesc>
constexpr bool is_const_v = is_const<FuncTypeDesc>::value;


////////////////////////////////////////////////////////////////////////////////
// compose a function type from a function_type_descriptor
template <class FuncTypeDesc>
struct compose;

template <class ReturnType, bool Constness, class... ParamTypes>
struct compose<function_type_descriptor<ReturnType, Constness, ParamTypes...>>
{
    typedef ReturnType type(ParamTypes...);
};

template <class ReturnType, class... ParamTypes>
struct compose<function_type_descriptor<ReturnType, true, ParamTypes...>>
{
    typedef ReturnType type(ParamTypes...) const;
};

template <class FuncTypeDesc>
using compose_t = typename compose<FuncTypeDesc>::type;


////////////////////////////////////////////////////////////////////////////////
// compose a member function pointer from function_type_descriptor
template <class FuncTypeDesc, class ObjectType>
struct compose_member_function_pointer;

template <class ReturnType,
          bool Constness,
          class... ParamTypes,
          class ObjectType>
struct compose_member_function_pointer<
    function_type_descriptor<ReturnType, Constness, ParamTypes...>,
    ObjectType>
{
    typedef ReturnType ObjectType::*type(ParamTypes...);
};


template <class ReturnType, class... ParamTypes, class ObjectType>
struct compose_member_function_pointer<
    function_type_descriptor<ReturnType, true, ParamTypes...>,
    ObjectType>
{
    typedef ReturnType ObjectType::*type(ParamTypes...) const;
};

template <class FuncTypeDesc, class ObjectType>
using compose_member_function_pointer_t =
    typename compose_member_function_pointer<FuncTypeDesc, ObjectType>::type;

////////////////////////////////////////////////////////////////////////////////
// decompose a function type to a function_type_descriptor
template <class FunctionType>
struct decompose;

template <class ReturnType, class... ParamTypes>
struct decompose<ReturnType(ParamTypes...)>
{
    typedef function_type_descriptor<ReturnType, false, ParamTypes...> type;
};

template <class ReturnType, class... ParamTypes>
struct decompose<ReturnType(ParamTypes...) const>
{
    typedef function_type_descriptor<ReturnType, true, ParamTypes...> type;
};

// specialization for member function pointer type
template <class FunType, class ObjectType>
struct decompose<FunType ObjectType::*>
{
    typedef typename decompose<FunType>::type type;
};

// specialization for function pointer type
template <class Fun>
struct decompose<Fun*>
{
    typedef typename decompose<Fun>::type type;
};

template <class FunctionType>
using decompose_t = typename decompose<FunctionType>::type;
} // namespace ft_descriptor
} // namespace useful
