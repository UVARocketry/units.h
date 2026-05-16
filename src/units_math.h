#ifndef UNITS_MATH_H
#define UNITS_MATH_H
#include "units_commonTypes.h"
#include "units_quantity.h"
#include <algorithm>

template <typename T>
Quantity<T> abs(Quantity<T> v) {
    return Quantity<T>(std::abs(v.value));
}

template <typename T>
Quantity<T> max(Quantity<T> v1, Quantity<T> v2) {
    return Quantity<T>(std::max(v1.value, v2.value));
}

template <typename T>
Quantity<T> min(Quantity<T> v1, Quantity<T> v2) {
    return Quantity<T>(std::min(v1.value, v2.value));
}

inline Level sin(Angle v) {
    return Level(std::sin(v.as<Radian>().value));
}

inline Level cos(Angle v) {
    return Level(std::cos(v.as<Radian>().value));
}
#endif
