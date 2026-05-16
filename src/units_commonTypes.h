#ifndef UNITS_COMMONTYPES_H
#define UNITS_COMMONTYPES_H

#include "units_baseTypeTags.h"
#include "units_names.h"
#include "units_quantity.h"

typedef Quantity<Meter> Length;
typedef Quantity<Kilogram> Mass;
typedef Quantity<Second> Time;

typedef Div<Meter, Second> VelocityType;
typedef Quantity<VelocityType> Velocity;

typedef Div<Meter, Mult<Second, Second>> AccelerationType;
typedef Quantity<AccelerationType> Acceleration;

typedef Quantity<Mult<Meter, Meter>> Area;
typedef Quantity<Mult<Meter, Meter, Meter>> Volume;
typedef Mult<Kilogram, Acceleration> Newton;
template <>
struct UnitName<Newton> {
    static constexpr inline StringView value = StringView::fromRawString("N");
};

typedef Quantity<Newton> Force;

typedef Div<Force, Area> Pascal;

template <>
struct UnitName<Pascal> {
    static constexpr inline StringView value = StringView::fromRawString("Pa");
};

typedef Quantity<Pascal> Pressure;

struct InchMercury {
    typedef Pascal ConversionTo;
    constexpr inline static ConversionFactor factor = ConversionFactor(3386.39);
    constexpr inline static const char* unitName = "inHg";
};

typedef Quantity<Kelvin> Temperature;
typedef Quantity<Div<Mass, Volume>> Density;
typedef Mult<Newton, Meter> Joule;
typedef Quantity<Joule> Work;
typedef Quantity<One> Constant;
typedef Quantity<Ampere> Current;
typedef Quantity<Mole> Substance;
typedef Quantity<Candela> LuminousIntensity;
typedef Div<Joule, Ampere> Volt;
typedef Quantity<Volt> Voltage;

struct Tesla {
    typedef Div<Force, Mult<Ampere, Meter>> ConversionTo;
    constexpr inline static ConversionFactor factor = ConversionFactor(1.0);
    constexpr inline static const char* unitName = "T";
};

typedef Quantity<Tesla> MagneticField;

struct Gauss {
    typedef Tesla ConversionTo;
    constexpr inline static ConversionFactor factor =
        ConversionFactor(1.0 / 10000.0);
    constexpr inline static const char* unitName = "G";
};

// typedef Quantity<Gauss> MagneticFieldGauss;

struct Hertz {
    typedef Div<One, Second> ConversionTo;
    constexpr inline static ConversionFactor factor = ConversionFactor(1.0);
    constexpr inline static const char* unitName = "Hz";
};

typedef Quantity<Radian> Angle;
typedef Div<Radian, Second> AngularVelocityType;
typedef Quantity<AngularVelocityType> AngularVelocity;
typedef Div<AngularVelocityType, Second> AngularAccelerationType;
typedef Quantity<AngularAccelerationType> AngularAcceleration;
typedef Quantity<Ratio> Level;
#endif
