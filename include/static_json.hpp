#pragma once

#include <iostream>
#include <sstream>


namespace useful
{
namespace static_json
{
namespace detail
{

template <class...>
struct type_list
{
};

////////////////////////////////////////////////////////////////////////////////
// get first type of a type_list
template <class TypeList>
struct head;

template <class First, class... Rest>
struct head<type_list<First, Rest...>>
{
    typedef First type;
};

template <class TypeList>
using head_t = typename head<TypeList>::type;


////////////////////////////////////////////////////////////////////////////////
// get tail of a type_list as another type_list
template <class TypeList>
struct tail;

template <class First, class... Rest>
struct tail<type_list<First, Rest...>>
{
    typedef type_list<Rest...> type;
};

template <class TypeList>
using tail_t = typename tail<TypeList>::type;


////////////////////////////////////////////////////////////////////////////////
// returns length of a type_list
template <class TypeList>
struct length;

template <class... Types>
struct length<type_list<Types...>>
{
    static const std::size_t value = sizeof...(Types);
};

template <class TypeList>
constexpr std::size_t length_v = length<TypeList>::value;


////////////////////////////////////////////////////////////////////////////////
// returns type at index in a type_list
template <class TypeList, std::size_t Index>
struct type_at_index : type_at_index<tail_t<TypeList>, Index - 1>
{
    static_assert(length_v<TypeList>> Index, "out of bounds access");
};

template <class TypeList>
struct type_at_index<TypeList, 0>
{
    typedef head_t<TypeList> type;
};


template <class TypeList, std::size_t Index>
using type_at_index_t = typename type_at_index<TypeList, Index>::type;


////////////////////////////////////////////////////////////////////////////////
// return index of first type encountered that matches T
template <class TypeList, class T, std::size_t Index = 0>
struct index_of_type;

template <class First, class... Rest, class T, std::size_t Index>
struct index_of_type<type_list<First, Rest...>, T, Index>
    : index_of_type<type_list<Rest...>, T, Index + 1>
{
};

template <class... Rest, class T, std::size_t Index>
struct index_of_type<type_list<T, Rest...>, T, Index>
{
    static const std::size_t value = Index;
};

template <class TypeList, class T>
constexpr std::size_t index_of_type_v = index_of_type<TypeList, T>::value;


template <class NameProvider, char C, std::size_t P>
struct static_string_length_
    : static_string_length_<NameProvider, NameProvider::value[P], P + 1>
{
};

template <class NameProvider, std::size_t P>
struct static_string_length_<NameProvider, '\0', P>
{
    static const std::size_t value = P;
};

template <class NameProvider>
constexpr std::size_t static_string_length_v =
    static_string_length_<NameProvider, NameProvider::value[0], 1>::value;

} // namespace detail


template <class NameProvider, class Type>
struct field
{
    Type value;

    std::string
    name() const
    {
        return NameProvider::value;
    }

    std::string
    str() const
    {
        return std::to_string(value);
    }
};

template <class NameProvider>
struct field<NameProvider, bool>
{
    bool value;


    std::string
    name() const
    {
        return NameProvider::value;
    }

    std::string
    str() const
    {
        if(value)
        {
            return "true";
        }

        return "false";
    }
};

template <class NameProvider>
struct field<NameProvider, std::string>
{
    std::string value;


    std::string
    name() const
    {
        return NameProvider::value;
    }

    std::string
    str() const
    {
        return value;
    }
};


template <class... Fields>
class json;

template <class NameProvider, class... Subfields>
struct field<NameProvider, json<Subfields...>>;


template <class... NameProviders, class... Types>
class json<field<NameProviders, Types>...> : field<NameProviders, Types>...
{
public:
    template <class NameProvider>
    struct type_from_name
    {
        typedef detail::type_at_index_t<
            detail::type_list<Types...>,
            detail::index_of_type_v<detail::type_list<NameProviders...>,
                                    NameProvider>>
            type;
    };

    template <class NameProvider>
    using type_from_name_t = typename type_from_name<NameProvider>::type;

private:
    template <class FirstField, class... RestFields>
    struct serialize_helper
    {
        static std::string
        dispatch_str(const json<field<NameProviders, Types>...>* self)
        {
            const auto field_ptr = static_cast<const FirstField*>(self);
            return field_ptr->name() + ": " + field_ptr->str() + ", " +
                   serialize_helper<RestFields...>::dispatch_str(self);
        };

        static void
        dispatch_stream_set(std::istream& in,
                            json<field<NameProviders, Types>...>* self)
        {
            in >> static_cast<FirstField*>(self)->value;
            in.ignore(std::numeric_limits<std::streamsize>::max(), ':');

            serialize_helper<RestFields...>::dispatch_stream_set(in, self);
        }
    };

    template <class LastField>
    struct serialize_helper<LastField>
    {
        static std::string
        dispatch_str(const json<field<NameProviders, Types>...>* self)
        {
            const auto field_ptr = static_cast<const LastField*>(self);
            return field_ptr->name() + ": " + field_ptr->str();
        };

        static void
        dispatch_stream_set(std::istream& in,
                            json<field<NameProviders, Types>...>* self)
        {
            in >> static_cast<LastField*>(self)->value;
        }
    };

public:
    template <class Name>
    type_from_name_t<Name>&
    get()
    {
        return static_cast<field<Name, type_from_name_t<Name>>*>(this)->value;
    }

    template <class Name>
    const type_from_name_t<Name>&
    get() const
    {
        return static_cast<const field<Name, type_from_name_t<Name>>*>(this)
            ->value;
    }

    std::string
    str() const
    {
        std::string out("{ ");

        out.append(
            serialize_helper<field<NameProviders, Types>...>::dispatch_str(
                this));

        out.append(" }");

        return out;
    }

    void
    stream_set(std::istream& in)
    {
        // strip beginning of json stream ie. "{ somename:"
        in.ignore(std::numeric_limits<std::streamsize>::max(), ':');

        serialize_helper<field<NameProviders, Types>...>::dispatch_stream_set(
            in, this);

        // ensure whole json message is consumed
        in.ignore(std::numeric_limits<std::streamsize>::max(), '}');
    }

private:
};

template <class NameProvider, class... Subfields>
struct field<NameProvider, json<Subfields...>>
{
    json<Subfields...> value;

    std::string
    name() const
    {
        return NameProvider::value;
    }

    std::string
    str() const
    {
        return value.str();
    }
};


template <class... Fields>
std::ostream&
operator<<(std::ostream& out, const json<Fields...>& obj)
{
    out << obj.str();
    return out;
}

template <class... Fields>
std::istream&
operator>>(std::istream& in, json<Fields...>& obj)
{
    obj.stream_set(in);

    return in;
}
} // namespace static_json
} // namespace useful
