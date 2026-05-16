# units.h

A c++17 library for all your unit needs

## Why

Are you tired of writing the following code and still not even sure if your dimensional analysis is correct?

```cpp
float airDensity_kgpm3(float baseBaro_Pa, float baseTemp_K, float basePos_m,
                   float h_m) {
    float g_mps2 = 9.80864;

    // gas constant for dry air [J/(kg·K)]
    float R_JpkgK = 287.05f;
    // temperature lapse rate [K/m]
    float L_Kpm = 0.0065f; 

    // Altitude difference
    float dz_m = (h_m - basePos_m);

    // Temperature at new altitude
    float T_K = baseTemp_K - L_Kpm * dz_m;

    // Pressure at new altitude
    float exponent = g_mps2 / (R_JpkgK * L_Kpm);
    float p_Pa = baseBaro_Pa * std::pow(T_K / baseTemp_K, exponent);

    // Density at new altitude
    float ret_kgpm3 = p_Pa / (R_JpkgK * T_K);
    return ret;
}
```

units.h is a library that checks all of that icky analysis *for you* at compile time. It allows you to express your unit types *in English*. Furthermore, it is *as fast* as using regular floating point values in when compiling at higher optimization levels. Just `#include "units.h"` and start converting your variables to units

## Quick Demo

units.h enables this to happen safely and be checked:

```cpp
Length l = 1.0_m;
Time t = 1.0_s;
// it understands operations between different unit types
Velocity v = l / t;
Length l2 = v * t;
// this will throw a compile time error
Velocity v2 = l * t;
```

## How

units.h distinguishes the following things: tag types and Quantity types. A tag type has no properties and is useless on its own, Quantity types are where the actual interesting stuff goes on and they are defined as a Quantity of a tag type (eg `Quantity<TagType>`)

The following tag types are avialable:

- `One`: Defines a unit less value (such as a ratio)
- `Meter`
- `Kilogram`
- `Ampere`
- `Mole`
- `Candela`
- `Second`
- `Kelvin`
- `Radian` (Equivalent to `One`)
- `Ratio` (Equivalent to `One`)
- `Newton` (Equivalent to kg*m/s^2)
- `Pascal` (Equivalent to N/m^2)
- `Tesla` (Equivalent to N/(A*m))
- `Hertz` (Equivalent to 1/s)

It also defines the following tags that are convertible to one of the above base tags:

- `Foot`
- `Gauss` (Convertible to `Tesla`)
- `InchMercury` (Convertible to `Pascal`)
- `Inch`
- `Gravity` (equal to 9.80665 m/s^2)
- `Minute`
- `Hour`
- `Celsius`
- `Fahrenheit`
- `Degree`
- `Revolution`
- `Percent` (convertible to `Ratio`)

It also defines the following public meta-tags (tags that compose other tags together):

- `Centi<>` (eg `Centi<Meter>`)
- `Deci<>`
- `Nano<>`
- `Micro<>`
- `Milli<>`
- `Tera<>`
- `Giga<>`
- `Mega<>`
- `Kilo<>`
- `AGL<>` (for "Above Ground Level", more on this later)
- `Mult<...>` (more on this later)
- `Div<N, D>` (more on this later)

`Quantity<>` takes a tag type and defines a Quantity on it. `Quantity<>` is the class that provides all of the operations to use. `Quantity<>` also provides a full set of operator overloads so that you can use it as if it were a floating point value. For most programs, you should just use the following pre-defined Quantity types for representing various things

- `Length`: `Quantity<Meter>`
- `Mass`: `Quantity<Kilogram>`
- `Time`: `Quantity<Second>`
- `Velocity`: `Quantity<m/s>`
- `Acceleration`: `Quantity<m/s^2>`
- `Area`: `Quantity<m*m>`
- `Volume`: `Quantity<m*m*m>`
- `Force`: `Quantity<Newton>`
- `Pressure`: `Quantity<Pascal>`
- `Temperature`: `Quantity<Kelvin>`
- `Density`: `Quantity<Mass/Volume>`
- `Work`: `Quantity<Joule>`
- `Constant`: `Quantity<One>`
- `Current`: `Quantity<Ampere>`
- `Substance`: `Quantity<Mole>`
- `LuminousIntensity`: `Quantity<Candela>`
- `Voltage`: `Quantity<Volt>`
- `MagneticField`: `Quantity<Tesla>`
- `Angle`: `Quantity<Radian>`
- `AngularVelocity`: `Quantity<Radian/Second>`
- `AngularAcceleration`: `Quantity<Radian/Second^2>`
- `Level`: `Quantity<Ratio>` (Describes something like "amounts of something". eg a servo that is 50% deployed means it is `Level(0.5) == Level::from<Percent>(50)`)

## Composing other units together

For the times when the basic units arent sufficient, you can compose other units together with the given meta-tags. 

The most basic unit tags are `Mult<>` and `Div<>`. These allow you to compose multiple different units together multiplicatively. For example, acceleration (m/s^2) is defined as `Div<Meter, Mult<Second, Second>>`. You can also mix and match multiplying and dividing tags and quantities, units.h transparently handles all of that: `Div<Kelvin, Length>` (for Lapse Rate (K/m)). 

Most of the other tags are for metric prefixes (eg centi-). For this, you just pass in a tag type (eg `Centi<Meter>`). The use of this will be more apparent later with `from()` and `as()`

A good exmple of composing these units is the definition for `mdps` and `mg` in `units_short.h`

## Creating Quantities

There are a few options for creating a value of a `Quantity<>` type. 

1. Use the constructor (not recommended)
2. Use `from<T>()`
3. Use literals

### Constructor

`Quantity<>` provides a `Quantity(float)` constructor that creates a Quantity from the given value, however, you should avoid using it as it assumes the internal tag type for the `Quantity<>`. For example, `Length(1)` would mean something very different if `Length` was defined as `Quantity<Centi<Meter>>`. This constructor should only be used in situations where the tag type is very obvious (eg when you are making your own tag type, see `airDensity` in `demo/main.cpp`). 

```cpp
Length v = Length(1.0);
// can also do
Length v2{1.0};
```

### `from<T>()`

`from<T>()` creates a quantity from the passed tag type and then converts it into the base tag type. See `main` in `demo/main.cpp` for an example.

```cpp
Length v = Length::from<Centi<Meter>>(100.0);
std::cout << v << std::endl; // prints "1 m"
```

### Literals

units.h defines a lot of literals for use with numbers. (See [user-defined literals](https://en.cppreference.com/cpp/language/user_literal) for help). 

```cpp
Length v = 1.0_m;
Length v2 = 100.0_cm;
std::cout << v << ", " << v2 << std::endl; // prints "1 m, 1 m"
```

The following literals are defined, along with their associate return type

- `_m`: `Length`
- `_cm`: `Length`
- `_in`: `Length`
- `_ft`: `Length`
- `_mps`: `Velocity`
- `_mps2`: `Acceleration`
- `_mg`: `Acceleration`
- `_g`: `Acceleration`
- `_rad`: `Angle`
- `_radps`: `AngularVelocity`
- `_mdps`: `AngularVelocity`
- `_ms`: `Time`
- `_s`: `Time`
- `_kg`: `Mass`
- `_Pa`: `Pressure`
- `_K`: `Temperature`
- `_C`: `Temperature`
- `_pct`: `Level`

## Using Quantities

The most basic thing to do with quantities is operations between them. The following operations are supported:

- scalar multiplication and division (does not change the quantity type)
- addition and subtraction between two quantities of the same type (does not change the quantity type)
- multiplication and division between any quantity types (returns a new quantity type)
- comparison between quantities (`<`, `>`, `<=`, `==`, etc)

```cpp
Length v{1};
// no change
Length v2 = v * 2;
// no change
Length v3 = v + v2;
// the type changes
Area a = v * v2;
```

Another use of the quantity is directly accessing the value stored inside using the `value` field. This is NOT recommended as it can break the assumptions that `Quantity<>` makes (it's like using `unsafe` in Rust).

## Converting to foreign units

The `as<T>()` function allows converting to a non-base unit. This is usually used only for printing and logging as it is best to keep everything in the base unit for performance

```cpp
Length l = 1.0_m;

auto l2 = l.as<Centi<Meter>>();
std::cout << l << ", " << l2 << std::endl; // prints "1 m, 100 cm"
```

## Above Ground Level

The distinction between whether a vertical value is "above ground level" or "above sea level" is *very* important in rocketry. If you're deploying airbrakes, basing your deployment level off of an ASL value will cause your rocket to massively undershoot.

units.h makes the assumption that all vertical values are ASL unless otherwise specified. Let me reiterate, units.h ***FORCES all of your vertical values to be ASL***

If you want to use an AGL value, cast your `Length` value to a `Quantity<AGL<Meter>>` with the helper functions `agl_from_asl` and `asl_from_agl`. These functions both take in a ground level ASL altitude as well:

```cpp
Length groundLevel = 100_m;

Length heightASL = 300_m;

Quantity<AGL<Meter>> heightAGL = agl_from_asl(heightASL, groundLevel);
std::cout << heightAGL; // prints "200 m agl"
Length heightASL2 = asl_from_agl(heightAGL, groundLevel);
```

`AGL<Meter>` values can also be transparently converted to other `AGL<>` tag types. 

```cpp
// 200 m agl -> 656.168 ft agl
Quantity<AGL<Foot>> heightFtAGL = heightAGL.as<AGL<Foot>>();
```

The biggest annoyance when dealing with AGL values is that you can not transparently combine them with other values. This annoyance *is intentional*: your values should not be stored as AGL. This can be an annoyance when taking a delta between two AGL values

```cpp
Quantity<AGL<Meter>> l1{1};
Quantity<AGL<Meter>> l2{2};

// logic says that a delta between two values should just be a Length, not an AGL length because it no longer represents a vertical value
Quantity<AGL<Meter>> delta = l2 - l1;
// to get past this, use .reinterpret()
Length actualDelta = delta.reinterpret<Meter>();
```

## Math

units.h currently provides the following math functions:

- `abs`: Takes the absolute value of any quantity
- `max`: Returns the maximum of two quantities
- `min`: Returns the minimum of two quantities
- `sin`: Takes the sin of an `Angle` and returns a `Level`
- `cos`: Takes the cos of an `Angle` and returns a `Level`

## Helpers

units.h current provides the following helper function:

- `unit_value_from<T2, T1>(v1)`: Converts a floating point value in the unit of `T1` into a floating point value of the unit of `T2` (eg `unit_value_from<Foot, Meter>(1) ~= 3.3`)
