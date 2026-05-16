#ifndef UNITS_SHORT_H
#define UNITS_SHORT_H
#include "units_commonTypes.h"

namespace shortunits {
typedef Milli<Div<Degree, Second>> mdps;
typedef Milli<Gravity> mg;
typedef Gravity g;
typedef Meter m;
typedef Centi<Meter> cm;
typedef Foot ft;
typedef Milli<Second> ms;
typedef Second s;
typedef Div<Meter, Second> mps;
typedef Div<Meter, Mult<Second, Second>> mps2;
typedef Div<Radian, Second> radps;
typedef Radian rad;
typedef Inch in;
typedef Kilogram kg;
}

constexpr AngularVelocity operator""_mdps(long double v) {
    return AngularVelocity::from<shortunits::mdps>(v);
}
constexpr Acceleration operator""_mg(long double v) {
    return Acceleration::from<shortunits::mg>(v);
}
constexpr Acceleration operator""_g(long double v) {
    return Acceleration::from<shortunits::g>(v);
}
constexpr Length operator""_m(long double v) {
    return Length::from<shortunits::m>(v);
}
constexpr Length operator""_cm(long double v) {
    return Length::from<shortunits::cm>(v);
}
constexpr Length operator""_ft(long double v) {
    return Length::from<shortunits::ft>(v);
}
constexpr Time operator""_ms(long double v) {
    return Time::from<shortunits::ms>(v);
}
constexpr Time operator""_s(long double v) {
    return Time::from<shortunits::s>(v);
}
constexpr Velocity operator""_mps(long double v) {
    return Velocity::from<shortunits::mps>(v);
}
constexpr Acceleration operator""_mps2(long double v) {
    return Acceleration::from<shortunits::mps2>(v);
}
constexpr AngularVelocity operator""_radps(long double v) {
    return AngularVelocity::from<shortunits::radps>(v);
}
constexpr Angle operator""_rad(long double v) {
    return Angle::from<shortunits::rad>(v);
}
constexpr Length operator""_in(long double v) {
    return Length::from<shortunits::in>(v);
}
constexpr Mass operator""_kg(long double v) {
    return Mass::from<shortunits::kg>(v);
}

// Pressure literals
constexpr Pressure operator""_Pa(long double v) {
    return Pressure::from<Pascal>(v);
}

// Temperature literals
constexpr Temperature operator""_K(long double v) {
    return Temperature::from<Kelvin>(v);
}
constexpr Temperature operator""_C(long double v) {
    return Temperature::from<Celsius>(v);
}

// Level/Ratio literals
constexpr Level operator""_pct(long double v) {
    return Level::from<Percent>(v);
}
#endif
