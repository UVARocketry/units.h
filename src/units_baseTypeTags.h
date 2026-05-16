#ifndef UNITS_BASETYPETAGS_H
#define UNITS_BASETYPETAGS_H
/*
 *
 * This file defines the basic tags to be used (eg Meter, Centi<>, etc)
 *
 */
#include "_units_constants.h"
#include "_units_simplificationPasses.h"
#include "units_conversions.h"
#include <cmath>

template <typename T>
struct Centi {
    typedef T ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(1.0 / 100.0);
    constexpr inline static const char* prefix = "c";
};

template <typename T>
struct Deci {
    typedef T ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(1.0 / 10.0);
    constexpr inline static const char* prefix = "d";
};

template <typename T>
struct Nano {
    typedef T ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(1.0 / 1000.0 / 1000.0 / 1000.0);
    constexpr inline static const char* prefix = "n";
};

template <typename T>
struct Micro {
    typedef T ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(1.0 / 1000.0 / 1000.0);
    constexpr inline static const char* prefix = "u";
};

template <typename T>
struct Milli {
    typedef T ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(1.0 / 1000.0);
    constexpr inline static const char* prefix = "m";
};

template <typename T>
struct Tera {
    typedef T ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(1000.0 * 1000.0 * 1000.0 * 1000.0);
    constexpr inline static const char* prefix = "T";
};

template <typename T>
struct Giga {
    typedef T ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(1000.0 * 1000.0 * 1000.0);
    constexpr inline static const char* prefix = "G";
};

template <typename T>
struct Mega {
    typedef T ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(1000.0 * 1000.0);
    constexpr inline static const char* prefix = "M";
};

template <typename T>
struct Kilo {
    typedef T ConversionTo;
    constexpr inline static ConversionFactor factor = ConversionFactor(1000.0);
    constexpr inline static const char* prefix = "k";
};

struct Meter {
    constexpr inline static const char* unitName = "m";
};

template <typename T>
struct AGL {
    typedef AGL<Meter> ConversionTo;

    // we create a custom conversion factor
    typedef ::ConversionTo<T, Meter> ToMeter;
    typedef ::ConversionTo<Meter, T> FromMeter;
    struct Factor {
        constexpr inline float applyTo(float v) const {
            return ToMeter::apply(v);
        }
        constexpr inline float applyInverseTo(float v) const {
            return FromMeter::apply(v);
        }
    };
    static constexpr inline Factor factor = Factor();
};

template <>
struct AGL<Meter> {};

struct Foot {
    typedef Meter ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(1.0 / 3.28084);

    constexpr inline static const char* unitName = "ft";
};

struct Inch {
    typedef Foot ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(1.0 / 12.0);

    constexpr inline static const char* unitName = "in";
};

struct Kilogram {
    constexpr inline static const char* unitName = "kg";
};

struct Ampere {
    constexpr inline static const char* unitName = "A";
};

struct Mole {
    constexpr inline static const char* unitName = "mol";
};

struct Candela {
    constexpr inline static const char* unitName = "cd";
};

struct Second {
    constexpr inline static const char* unitName = "s";
};

/// the type name for g's of acceleration (eg 5 g's)
struct Gravity {
    typedef Div<Meter, Mult<Second, Second>> ConversionTo;
    constexpr inline static const char* unitName = "g";
    constexpr inline static ConversionFactor factor =
        ConversionFactor(gravityAtSeaLevel);
};

struct Minute {
    typedef Second ConversionTo;
    constexpr inline static ConversionFactor factor = ConversionFactor(60.0);
    constexpr inline static const char* unitName = "min";
};

struct Hour {
    typedef Second ConversionTo;
    constexpr inline static ConversionFactor factor = ConversionFactor(3600.0);
    constexpr inline static const char* unitName = "hr";
};

struct Kelvin {
    constexpr inline static const char* unitName = "K";
};
struct Celsius {
    typedef Kelvin ConversionTo;
    constexpr inline static const char* unitName = "C";
    constexpr inline static ConversionFactor factor =
        ConversionFactor(273.15, 1.0);
};
struct Fahrenheit {
    typedef Celsius ConversionTo;
    constexpr inline static const char* unitName = "F";
    constexpr inline static ConversionFactor factor =
        ConversionFactor(-32, 5.0 / 9.0);
};

struct Radian {
    // Radians are technically unitless, but we will pretend that isnt
    // true unless the user explicitly tries to cast
    typedef One ConversionTo;
    constexpr inline static ConversionFactor factor = ConversionFactor(1.0);
    constexpr inline static const char* unitName = "rad";
};

struct Degree {
    typedef Radian ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(3.14159265359f / 180.0f);
    constexpr inline static const char* unitName = "deg";
};

struct Revolution {
    typedef Radian ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(2 * M_PI);
    constexpr inline static const char* unitName = "rev";
};

/// Ratio represents a dimensionless value in the range [0, 1], typically the
/// result of dividing two quantities of the same unit (e.g., efficiency,
/// percentage/100). For displaying percentages, see @ref Percent.
struct Ratio {
    typedef One ConversionTo;
    constexpr inline static ConversionFactor factor = ConversionFactor(1.0);
    constexpr inline static const char* unitName = "";
};

/// Percent represents a percentage value (e.g., 50 for 50%). It converts to
/// @ref Ratio by dividing by 100, so Quantity<Percent>(50) becomes
/// Quantity<Ratio>(0.5). For storing ratios as 0-1 values directly, use @ref
/// Ratio instead.
struct Percent {
    typedef Ratio ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(1.0 / 100.0);
    constexpr inline static const char* unitName = "%";
};
#endif
