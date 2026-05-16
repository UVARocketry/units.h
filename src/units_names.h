#ifndef UNITS_NAMES_H
#define UNITS_NAMES_H
/*
 *
 * Defines helper classes for getting the name of a unit
 *
 */

#include "units_stringView.h"

template <typename T, class = void>
struct UnitName {
    static constexpr inline StringView value = StringView::fromRawString("?");
};

/// Specialization that sets name = T::unitName when T has a unitName member
template <typename T>
struct UnitName<T, std::void_t<decltype(T::unitName)>> {
    static constexpr inline StringView value =
        StringView::fromRawString(T::unitName);
};

template <typename T, class = void>
struct UnitPrefix {
    static constexpr inline StringView value = StringView::fromRawString("?");
};

/// Specialization that sets name = T::unitName when T has a unitName member
template <typename T>
struct UnitPrefix<T, std::void_t<decltype(T::prefix)>> {
    static constexpr inline StringView value =
        StringView::fromRawString(T::prefix);
};
#endif
