#include "units.h"
#include <iostream>

// demo functions from the codebase
Length baroToAltitude(Pressure baro) {

    static const Pressure P0 = 101325.0_Pa;
    static const float exponent = 0.190263;
    static const Length mult{44330.7692308};

    Length ret = mult * (1.0 - pow((baro / P0).value, exponent));
    return ret;
}

Density airDensity(Pressure baseBaro, Temperature baseTemp, Length basePos,
                   Length h) {
    Acceleration g = 1.0_g;

    // constexpr float g = 9.80665f; // gravity [m/s^2]
    typedef Quantity<Div<Joule, Mult<Kilogram, Kelvin>>> RType;
    // gas constant for dry air [J/(kg·K)]
    // use the Quantity(float) constructor here
    constexpr RType R{287.05f};

    typedef Quantity<Div<Kelvin, Meter>> LapseRate;
    constexpr LapseRate L{0.0065f}; // temperature lapse rate [K/m]

    // Reference conditions from base state
    Pressure p0 = baseBaro;
    Temperature T0 = baseTemp;
    Length z0 = basePos;

    // Altitude difference
    Length dz = (h - z0);

    // Temperature at new altitude
    Temperature T = T0 - L * dz;

    // Pressure at new altitude
    Constant exponent = g / (R * L);
    Pressure p = p0 * std::pow((T / T0).value, exponent.value);

    // Density at new altitude
    Density ret = p / (R * T);
    return ret;
}

int main() {

    // test that things work properly
    Length l = Length::from<Meter>(1);
    Length l2 = 100.0_cm;

    std::cout << l << ", " << l2 << std::endl;      // should both print "1 m"
    std::cout << l.as<Centi<Meter>>() << std::endl; // should print "100 cm"

    Time t = 1.0_s;

    Velocity v = l / t;

    std::cout << v << std::endl; // should print "1 m/s"

    Level a = 10._pct;
    std::cout << v * a.as<Constant>() << std::endl;
}
