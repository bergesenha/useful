#pragma once


namespace useful
{
template <class MemberVariableType>
struct deduce_member_variable_type;

template <class VariableType, class ObjectType>
struct deduce_member_variable_type<VariableType ObjectType::*>
{
    typedef VariableType type;
};

template <class MemberVariableType>
using deduce_member_variable_type_t =
    typename deduce_member_variable_type<MemberVariableType>::type;


template <class MemberVariableType>
struct deduce_member_variable_object_type;

template <class VariableType, class ObjectType>
struct deduce_member_variable_object_type<VariableType ObjectType::*>
{
    typedef ObjectType type;
};

template <class MemberVariableType>
using deduce_member_variable_object_type_t =
    typename deduce_member_variable_object_type<MemberVariableType>::type;
} // namespace useful
