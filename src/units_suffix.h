#ifndef UNITS_SUFFIX_H
#define UNITS_SUFFIX_H
// Helper to build suffix from prefix + name
#include "units_baseTypeTags.h"
#include "units_defs.h"
#include "units_names.h"
#include "units_stringView.h"

#ifndef UNITS_STRING_T
#include <string>
#define UNITS_STRING_T std::string
#endif

template <typename T>
inline UNITS_STRING_T unitNameWithPrefix() {
    constexpr StringView prefix = UnitPrefix<T>::value;
    constexpr StringView name = UnitName<T>::value;
    UNITS_STRING_T result;
    if (prefix.length > 0 && prefix.data[0] != '?') {
        result += UNITS_STRING_T(prefix.data, prefix.length);
    }
    if (name.length > 0 && name.data[0] != '?') {
        result += UNITS_STRING_T(name.data, name.length);
    }
    return result;
}

// Use struct with static method since C++ doesn't allow partial func template
// specialization
template <typename T>
struct UnitSuffix {
    static UNITS_STRING_T get() {
        return unitNameWithPrefix<T>();
    }
};

template <typename T>
inline UNITS_STRING_T getUnitSuffix() {
    constexpr StringView prefix = UnitPrefix<T>::value;
    constexpr StringView name = UnitName<T>::value;
    if constexpr (prefix.length == 1 && prefix.data[0] == '?') {
        // no prefix
        if constexpr (name.length == 1 && name.data[0] == '?') {
            return UnitSuffix<T>::get();
        } else {
            return UNITS_STRING_T(name.data, name.length);
        }
    } else {
        if constexpr (name.length == 1 && name.data[0] == '?') {
            return UnitSuffix<T>::get();
        } else {
            return UNITS_STRING_T(prefix.data, prefix.length) +
                   UNITS_STRING_T(name.data, name.length);
        }
    }
}

template <>
struct UnitSuffix<One> {
    static UNITS_STRING_T get() {
        return "";
    }
};

template <>
struct UnitSuffix<_Mult<>> {
    static UNITS_STRING_T get() {
        return "";
    }
};

template <typename T>
struct UnitSuffix<_Mult<T>> {
    static UNITS_STRING_T get() {
        return getUnitSuffix<T>();
    }
};

// Square: _Mult<T, T, Rest...> - two consecutive identical types at start
template <typename T, typename... Rest>
struct UnitSuffix<_Mult<T, T, Rest...>> {
    static UNITS_STRING_T get() {
        return getUnitSuffix<T>() + "2" + getUnitSuffix<_Mult<Rest...>>();
    }
};

// Cube: _Mult<T, T, T, Rest...> - three consecutive identical types at start
template <typename T, typename... Rest>
struct UnitSuffix<_Mult<T, T, T, Rest...>> {
    static UNITS_STRING_T get() {
        return getUnitSuffix<T>() + "3" + getUnitSuffix<_Mult<Rest...>>();
    }
};

// Quartic: _Mult<T, T, T, T, Rest...> - four consecutive identical types at
// start
template <typename T, typename... Rest>
struct UnitSuffix<_Mult<T, T, T, T, Rest...>> {
    static UNITS_STRING_T get() {
        return getUnitSuffix<T>() + "4" + getUnitSuffix<_Mult<Rest...>>();
    }
};

// General variadic case: pop off first arg, add its suffix to the rest
template <typename First, typename... Rest>
struct UnitSuffix<_Mult<First, Rest...>> {
    static UNITS_STRING_T get() {
        return getUnitSuffix<First>() + getUnitSuffix<_Mult<Rest...>>();
    }
};

template <typename A>
struct UnitSuffix<AGL<A>> {
    static UNITS_STRING_T get() {
        return getUnitSuffix<A>() + "_agl";
    }
};

// Single _Div specialization
template <typename A, typename B>
struct UnitSuffix<_Div<A, B>> {
    static UNITS_STRING_T get() {
        return getUnitSuffix<A>() + "p" + getUnitSuffix<B>();
    }
};
#endif
