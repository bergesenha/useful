#pragma once

#include <vector>
#include <tuple>
#include <utility>


namespace useful
{

namespace soa_detail
{
template <class MemberPointerType>
struct deduce_member_pointer;

template <class MemberType, class ObjectType>
struct deduce_member_pointer<MemberType ObjectType::*>
{
    typedef MemberType member_type;
    typedef ObjectType object_type;
};

template <class MemberPointerType>
using member_type_t =
    typename deduce_member_pointer<MemberPointerType>::member_type;

template <class MemberPointerType>
using object_type_t =
    typename deduce_member_pointer<MemberPointerType>::object_type;
} // namespace soa_detail


template <class PointerType, PointerType PointerValue>
struct member_reference
{
    static constexpr PointerType pointer_value = PointerValue;
};


template <class T, class... MemberRefs>
class element_proxy;

template <class T, class... PointerTypes, PointerTypes... PointerValues>
class element_proxy<T, member_reference<PointerTypes, PointerValues>...>
{
    typedef std::tuple<soa_detail::member_type_t<PointerTypes>*...> tuple_type;

    template <std::size_t S, class FirstRef, class... RestRefs>
    struct assignment_helper
    {
        static void
        assign(std::tuple<soa_detail::member_type_t<PointerTypes>*...>& tpl,
               const T& value)
        {
            *std::get<S>(tpl) = value.*FirstRef::pointer_value;

            assignment_helper<S + 1, RestRefs...>::assign(tpl, value);
        }
    };

    template <std::size_t S, class LastRef>
    struct assignment_helper<S, LastRef>
    {

        static void
        assign(std::tuple<soa_detail::member_type_t<PointerTypes>*...>& tpl,
               const T& value)
        {
            *std::get<S>(tpl) = value.*LastRef::pointer_value;
        }
    };

    template <std::size_t S, class FirstRef, class... RestRefs>
    struct make_helper
    {
        static void
        apply(T& v, const tuple_type& tpl)
        {
            v.*FirstRef::pointer_value = *std::get<S>(tpl);
            make_helper<S + 1, RestRefs...>::apply(v, tpl);
        }
    };

    template <std::size_t S, class LastRef>
    struct make_helper<S, LastRef>
    {
        static void
        apply(T& v, const tuple_type& tpl)
        {
            v.*LastRef::pointer_value = *std::get<S>(tpl);
        }
    };

public:
    element_proxy(soa_detail::member_type_t<PointerTypes>&... args)
        : pointers_(&args...)
    {
    }

    operator T() const
    {
        T out{};
        make_helper<0, member_reference<PointerTypes, PointerValues>...>::apply(
            out, pointers_);
        return out;
    }

    element_proxy&
    operator=(const T& value)
    {
        assignment_helper<0, member_reference<PointerTypes, PointerValues>...>::
            assign(pointers_, value);

        return *this;
    }

private:
    tuple_type pointers_;
};


// helper to access respective parents that are multiply inherited
template <class FirstParent, class... RestParents>
struct parent_helper
{
    template <class SoaType>
    static std::size_t
    size(const SoaType& s)
    {
        return s.FirstParent::members_.size();
    }

    template <class SoaType>
    static void
    push_back(SoaType& s, const typename SoaType::value_type& value)
    {
        s.FirstParent::members_.push_back(value.*FirstParent::pointer_value);
        parent_helper<RestParents...>::push_back(s, value);
    }

    template <class SoaType, class FirstElem, class... RestElems>
    static void
    push_back_members(SoaType& s, FirstElem&& f_elm, RestElems&&... rest)
    {
        s.FirstParent::members_.push_back(std::forward<FirstElem>(f_elm));
        parent_helper<RestParents...>::push_back_members(
            s, std::forward<RestElems>(rest)...);
    }

    template <class SoaType>
    static void
    reserve(SoaType& s, std::size_t n)
    {
        s.FirstParent::members_.reserve(n);
        parent_helper<RestParents...>::reserve(s, n);
    }
};


template <class LastParent>
struct parent_helper<LastParent>
{
    template <class SoaType>
    static std::size_t
    size(const SoaType& s)
    {
        return s.LastParent::members_.size();
    }

    template <class SoaType>
    static void
    push_back(SoaType& s, const typename SoaType::value_type& value)
    {
        s.LastParent::members_.push_back(value.*LastParent::pointer_value);
    }

    template <class SoaType, class LastElem>
    static void
    push_back_members(SoaType& s, LastElem&& elem)
    {
        s.LastParent::members_.push_back(std::forward<LastElem>(elem));
    }

    template <class SoaType>
    static void
    reserve(SoaType& s, std::size_t n)
    {
        s.LastParent::members_.reserve(n);
    }
};


template <class MemberPointerType, MemberPointerType MemberPointerValue>
class member_container
{
public:
    typedef soa_detail::member_type_t<MemberPointerType> member_type;
    typedef soa_detail::object_type_t<MemberPointerType> object_type;

    static constexpr MemberPointerType pointer_value = MemberPointerValue;

public:
    member_container() = default;

    std::vector<member_type> members_;
};


template <class T, class... MemberContainer>
class soa;

template <class T, class... MemberPtrTypes, MemberPtrTypes... MemberPtrValues>
class soa<T, member_container<MemberPtrTypes, MemberPtrValues>...>
    : public member_container<MemberPtrTypes, MemberPtrValues>...
{
public:
    typedef T value_type;
    typedef element_proxy<T,
                          member_reference<MemberPtrTypes, MemberPtrValues>...>
        proxy_value_type;

public:
    soa() : member_container<MemberPtrTypes, MemberPtrValues>()...
    {
    }

    std::size_t
    size() const
    {
        return parent_helper<
            member_container<MemberPtrTypes, MemberPtrValues>...>::size(*this);
    }

    void
    push_back(const T& value)
    {
        parent_helper<member_container<MemberPtrTypes, MemberPtrValues>...>::
            push_back(*this, value);
    }

    template <class... Elems>
    void
    push_back_members(Elems&&... elems)
    {
        parent_helper<member_container<MemberPtrTypes, MemberPtrValues>...>::
            push_back_members(*this, std::forward<Elems>(elems)...);
    }

    proxy_value_type operator[](std::size_t n)
    {
        return proxy_value_type(
            member_container<MemberPtrTypes, MemberPtrValues>::members_[n]...);
    }

    const proxy_value_type operator[](std::size_t n) const
    {
        return proxy_value_type(
            member_container<MemberPtrTypes, MemberPtrValues>::members_[n]...);
    }

    template <class MemberPtrType, MemberPtrType MemberPtrValue>
    soa_detail::member_type_t<MemberPtrType>*
    data()
    {
        return member_container<MemberPtrType, MemberPtrValue>::members_.data();
    }

    template <class MemberPtrType, MemberPtrType MemberPtrValue>
    const soa_detail::member_type_t<MemberPtrType>*
    data() const
    {
        return member_container<MemberPtrType, MemberPtrValue>::members_.data();
    }

    void
    reserve(std::size_t n)
    {
        parent_helper<member_container<MemberPtrTypes,
                                       MemberPtrValues>...>::reserve(*this, n);
    }
};
} // namespace useful
