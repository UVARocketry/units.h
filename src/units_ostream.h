#ifndef UNITS_OSTREAM_H
#define UNITS_OSTREAM_H
#include "units_baseTypeTags.h"
#include "units_defs.h"
#include "units_names.h"
#include "units_stringView.h"
#include "units_utils.h"

#ifndef UNITS_PRINTER_T
#include <ostream>
#define UNITS_PRINTER_T std::ostream
#endif

inline UNITS_PRINTER_T& operator<<(std::ostream& cout, StringView v) {
    for (size_t i = 0; i < v.length; i++) {
        cout << v.data[i];
    }
    return cout;
}

template <typename T>
void printUnitName(UNITS_PRINTER_T& cout) {
    constexpr StringView unitName = UnitName<T>::value;
    constexpr StringView prefix = UnitPrefix<T>::value;

    if constexpr (prefix.length == 1 && prefix.data[0] == '?') {
        if constexpr (unitName.length == 1 && unitName.data[0] == '?') {
            T o;
            cout << o;
        } else {
            cout << unitName;
        }
    } else {
        cout << prefix;
        printUnitName<typename ExtractInnerType<T>::Out>(cout);
    }
}

template <typename L, typename... Rest>
UNITS_PRINTER_T& operator<<(std::ostream& cout, _Mult<L, L, Rest...>) {
    _Mult<Rest...> r;
    printUnitName<L>(cout);
    cout << "^2";
    return cout;
}

template <typename L, typename R, typename... Rest>
UNITS_PRINTER_T& operator<<(std::ostream& cout, _Mult<L, R, Rest...>) {
    printUnitName<L>(cout);
    _Mult<R, Rest...> r;
    cout << "*" << r;
    return cout;
}

template <typename V>
UNITS_PRINTER_T& operator<<(std::ostream& cout, _Mult<V>) {
    printUnitName<V>(cout);
    return cout;
}

template <typename L, typename R>
UNITS_PRINTER_T& operator<<(std::ostream& cout, _Div<L, R>) {
    printUnitName<L>(cout);
    cout << "/";
    printUnitName<R>(cout);
    return cout;
}
template <typename L, typename... RMult>
UNITS_PRINTER_T& operator<<(std::ostream& cout, _Div<L, _Mult<RMult...>>) {
    printUnitName<L>(cout);
    cout << "/(";
    printUnitName<_Mult<RMult...>>(cout);
    cout << ")";
    return cout;
}

template <typename T>
UNITS_PRINTER_T& operator<<(std::ostream& cout, AGL<T>) {
    cout << T::unitName << " AGL";
    return cout;
}

inline UNITS_PRINTER_T& operator<<(std::ostream& cout, AGL<Meter>) {
    cout << "m AGL";
    return cout;
}

template <typename T>
UNITS_PRINTER_T& operator<<(std::ostream& cout, Quantity<T> q) {
    cout << q.value << " ";
    printUnitName<T>(cout);
    return cout;
}
#endif
