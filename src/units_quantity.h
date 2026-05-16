#ifndef UNITS_QUANTITY_H
#define UNITS_QUANTITY_H
#include "_units_simplificationPasses.h"
#include "units_conversions.h"

template <typename T>
struct Quantity {
    typedef T Formula;
    float value;

    // TODO: make this private (force users to explicitly do Quantity::from or
    // something that explicitly states the unit they are working with)
    explicit inline constexpr Quantity<T>(float v) : value(v) {
    }

    inline constexpr Quantity<T>() : value(0.0) {
    }

    inline constexpr Quantity<T> operator+(Quantity<T> v) const {
        return Quantity<T>(v.value + value);
    }
    inline constexpr Quantity<T>& operator+=(Quantity<T> v) {
        value += v.value;
        return *this;
    }

    inline constexpr Quantity<T> operator-() const {
        return Quantity<T>(-value);
    }
    inline constexpr Quantity<T> operator-(Quantity<T> v) const {
        return Quantity<T>(value - v.value);
    }
    inline constexpr Quantity<T>& operator-=(Quantity<T> v) {
        value -= v.value;
        return *this;
    }

    template <typename O>
    inline constexpr Quantity<Mult2<T, O>> operator*(Quantity<O> v) const {
        return Quantity<Mult2<T, O>>(v.value * value);
    }
    inline constexpr Quantity<T> operator*(float v) const {
        return Quantity<T>(v * value);
    }

    inline constexpr Quantity<T>& operator*=(float v) {
        value *= v;
        return *this;
    }
    inline constexpr Quantity<T>& operator*=(Quantity<One> v) {
        value *= v.value;
        return *this;
    }

    friend inline constexpr Quantity<T> operator*(float v, Quantity<T> v2) {
        return Quantity<T>(v * v2.value);
    }

    template <typename O>
    inline constexpr Quantity<Div2<T, O>> operator/(Quantity<O> v) const {
        return Quantity<Div2<T, O>>(value / v.value);
    }

    inline constexpr Quantity<T> operator/(float v) const {
        return Quantity<T>(value / v);
    }

    inline constexpr Quantity<T>& operator/=(float v) {
        value /= v;
        return *this;
    }

    friend inline constexpr Quantity<Div2<One, T>> operator/(float v,
                                                             Quantity<T> v2) {
        return Quantity<Div2<One, T>>(v / v2.value);
    }

    inline constexpr Quantity<Div2<One, T>> inverse() const {
        return Quantity<Div2<One, T>>(1.0 / value);
    }

    inline constexpr bool operator>(Quantity<T> v2) const {
        return value > v2.value;
    }
    inline constexpr bool operator==(Quantity<T> v2) const {
        return value == v2.value;
    }
    inline constexpr bool operator!=(Quantity<T> v2) const {
        return value != v2.value;
    }
    inline constexpr bool operator>=(Quantity<T> v2) const {
        return value >= v2.value;
    }
    inline constexpr bool operator<=(Quantity<T> v2) const {
        return value <= v2.value;
    }
    inline constexpr bool operator<(Quantity<T> v2) const {
        return value < v2.value;
    }

    /// Casts the quantity to a non-canonical quantity type
    /// (usually used for printing).
    ///
    /// Example:
    /// ```cpp
    /// Length v = Length::from<Meter>(1);
    /// stl::print << v; // prints "1 m"
    /// stl::print << v.as<Foot>(); // prints "3.28 ft"
    /// ```
    template <typename O>
    inline constexpr Quantity<ExtractQuantityT<O>> as() const {
        return Quantity<ExtractQuantityT<O>>(
            ConversionTo<T, ExtractQuantityT<O>>::apply(value));
    }

    /// WARN: THIS FUNCTION CHANGES THE UNIT WITHOUT ANY CASTING
    /// ONLY USE IF YOU ARE SURE THE UNDERLYING VALUES CAN BE CONVERTED
    ///
    /// Pretends the value in the quantity is actually a different type and does
    /// not apply any conversion
    ///
    /// Example:
    /// ```cpp
    /// Length v = Length::from<Meter>(1);
    /// Quantity<Foot> q = v.reinterpret<Foot>();
    /// assert(q.value == 1.0);
    /// ```
    template <typename O>
    inline constexpr Quantity<O> reinterpret() const {
        return Quantity<O>(value);
    }

    /// Converts a numerical value from the specified target unit into the
    /// target type
    ///
    /// Example:
    /// ```cpp
    /// Length v = Length::from<Foot>(3.28);
    /// Length q = Length::from<Meter>(1.0);
    /// assert(v == q);
    /// ```
    template <typename SourceUnit>
    static inline constexpr Quantity<T> from(float v) {
        return Quantity<T>(ConversionTo<SourceUnit, T>::apply(v));
    }

    inline constexpr explicit operator float() const {
        return value;
    }
};
#endif
