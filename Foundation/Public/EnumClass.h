#pragma once
#include <type_traits>

#define FROM_UNDERLYING(t, x) static_cast<t>(x)
#define TO_UNDERLYING(t, x) static_cast<typename std::underlying_type<t>::type>(x)

#define DEFINE_ENUM_CLASS_BITWISE_OPERATORS(type)\
inline constexpr type operator&(const type& lhs, const type& rhs)\
{\
    return FROM_UNDERLYING(type, TO_UNDERLYING(type, lhs) & TO_UNDERLYING(type, rhs));\
}\
inline constexpr type operator|(const type& lhs, const type& rhs)\
{\
    return FROM_UNDERLYING(type, TO_UNDERLYING(type, lhs) | TO_UNDERLYING(type, rhs));\
}\
inline constexpr type operator^(const type& lhs, const type& rhs)\
{\
    return FROM_UNDERLYING(type, TO_UNDERLYING(type, lhs) ^ TO_UNDERLYING(type, rhs));\
}\
inline constexpr type operator~(const type& a)\
{\
    return FROM_UNDERLYING(type, ~TO_UNDERLYING(type, a));\
}\
inline constexpr bool operator!(const type& a)\
{\
    return !TO_UNDERLYING(type, a);\
}\
inline type& operator&=(type& a, const type& b)\
{\
    a = a & b;\
    return a;\
}\
inline type& operator|=(type& a, const type& b)\
{\
    a = a | b;\
    return a;\
}\
inline type& operator^=(type& a, const type& b)\
{\
    a = a ^ b;\
    return a;\
}

template <typename enum_t>
inline bool Any(enum_t flags, enum_t compare)
{
    return (TO_UNDERLYING(enum_t, flags) & TO_UNDERLYING(enum_t, compare)) != 0;
}

//TODO: finish this
#define DEFINE_ENUM_CLASS_COMPARISON_OPERATORS(type)\
inline bool operator<(const type& a, const type& b)\
{\
    return TO_UNDERLYING(type, a) < TO_UNDERLYING(type, b);\
}
