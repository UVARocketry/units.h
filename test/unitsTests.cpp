#include "unitsTests.h"
#include "units.h"
#include <cassert>
#include <cmath>

// Helper to check if float values are approximately equal
bool approxEqual(float a, float b, float epsilon = 0.0001f) {
    return std::fabs(a - b) < epsilon;
}

extern "C" {

void units_testPrefixCasts() {
    assert(approxEqual(Length::from<Deci<Meter>>(10.0).value, 1.0));
    assert(approxEqual(Length::from<Centi<Meter>>(100.0).value, 1.0));
    assert(approxEqual(Length::from<Milli<Meter>>(1.0e3).value, 1.0));
    assert(approxEqual(Length::from<Micro<Meter>>(1.0e6).value, 1.0));
    assert(approxEqual(Length::from<Nano<Meter>>(1.0e9).value, 1.0));

    assert(approxEqual(Length::from<Kilo<Meter>>(1.0).value, 1.0e3));
    assert(approxEqual(Length::from<Mega<Meter>>(1.0).value, 1.0e6));
    assert(approxEqual(Length::from<Giga<Meter>>(1.0).value, 1.0e9));
    assert(approxEqual(Length::from<Tera<Meter>>(1.0).value, 1.0e12));
}

void units_testLengthConversions() {
    // Basic length conversions
    Length m{1.0f}; // 1 meter
    assert(approxEqual(m.as<Centi<Meter>>().value, 100.0f));
    assert(approxEqual(m.as<Milli<Meter>>().value, 1000.0f));
    assert(approxEqual(m.as<Kilo<Meter>>().value, 0.001f));
    assert(approxEqual(m.as<Inch>().value, 39.3701f));
    assert(approxEqual(m.as<Foot>().value, 3.28084f));

    // Reverse conversions using Quantity::from
    // 100 centimeters = 1 meter
    Length from_cm = Length::from<Centi<Meter>>(100.0f);
    assert(approxEqual(from_cm.value, 1.0f));

    // ~3.28084 feet = 1 meter
    Length from_ft = Length::from<Foot>(3.28084f);
    assert(approxEqual(from_ft.value, 1.0f, 0.01f));

    // Multi-step conversions
    // 12 inches = 1 foot
    Length from_in = Length::from<Inch>(12.0f);
    assert(approxEqual(from_in.as<Foot>().value, 1.0f));
}

void units_testTimeConversions() {
    // Basic time conversions
    Time s{1.0f}; // 1 second
    assert(approxEqual(s.as<Minute>().value, 1.0f / 60.0f));
    assert(approxEqual(s.as<Hour>().value, 1.0f / 3600.0f));

    // Reverse conversions using Quantity::from
    // 1 minute = 60 seconds
    Time from_min = Time::from<Minute>(1.0f);
    assert(approxEqual(from_min.value, 60.0f));

    // 1 hour = 3600 seconds
    Time from_hr = Time::from<Hour>(1.0f);
    assert(approxEqual(from_hr.value, 3600.0f));
    assert(approxEqual(from_hr.as<Minute>().value, 60.0f));
}

void units_testMassConversions() {
    // Mass conversions (currently just Kilogram as base)
    Mass kg{1.0f};
    // Kilogram is the base, so converting to itself should give 1.0
    assert(approxEqual(kg.as<Kilogram>().value, 1.0f));
}

void units_testAngleConversions() {
    // Angle conversions from radians
    Angle rad{1.0f}; // 1 radian
    assert(approxEqual(rad.as<Degree>().value, 57.2958f, 0.01f));
    assert(approxEqual(rad.as<Revolution>().value,
                       1.0f / (2.0f * 3.14159265359f), 0.001f));

    // Reverse conversions using Quantity::from
    // 180 degrees = pi radians
    Angle from_deg = Angle::from<Degree>(180.0f);
    assert(approxEqual(from_deg.as<Radian>().value, 3.14159265359f, 0.01f));

    // 1 revolution = 2*pi radians
    Angle from_rev = Angle::from<Revolution>(1.0f);
    assert(
        approxEqual(from_rev.as<Radian>().value, 2.0f * 3.14159265359f, 0.01f));
}

void units_testRatioAndPercentConversions() {
    // Ratio is base type (0-1), Percent converts to it
    Level ratio{0.5f}; // 0.5 ratio
    assert(approxEqual(ratio.as<Percent>().value, 50.0f));

    Quantity<Percent> pct{50.0f}; // 50%
    assert(approxEqual(pct.as<Ratio>().value, 0.5f));

    // Full range
    Level full{1.0f};
    assert(approxEqual(full.as<Percent>().value, 100.0f));

    Level zero{0.0f};
    assert(approxEqual(zero.as<Percent>().value, 0.0f));
}

void units_testAreaConversions() {
    // Area conversions: m^2 to cm^2
    Area m2{1.0f}; // 1 square meter
    assert(
        approxEqual(m2.as<Mult<Centi<Meter>, Centi<Meter>>>().value, 10000.0f));

    // Larger area
    Area m2_2{2.0f}; // 2 square meters = 20000 cm^2
    assert(approxEqual(m2_2.as<Mult<Centi<Meter>, Centi<Meter>>>().value,
                       20000.0f));

    // Reverse
    Quantity<Mult<Centi<Meter>, Centi<Meter>>> cm2{10000.0f};
    assert(approxEqual(cm2.as<Mult<Meter, Meter>>().value, 1.0f));
}

void units_testVelocityConversions() {
    // Velocity: m/s to cm/s
    Velocity ms{1.0f}; // 1 m/s
    assert(approxEqual(ms.as<Div<Centi<Meter>, Second>>().value, 100.0f));

    // Velocity in km/h
    assert(approxEqual(ms.as<Div<Kilo<Meter>, Hour>>().value, 3.6f, 0.01f));

    // Reverse using Quantity::from
    // 100 cm/s = 1 m/s
    Velocity from_cms = Velocity::from<Div<Centi<Meter>, Second>>(100.0f);
    assert(approxEqual(from_cms.value, 1.0f));
}

void units_testAccelerationConversions() {
    // Acceleration: m/s^2 to cm/s^2
    Acceleration acc{1.0f}; // 1 m/s^2

    // Converting to same structure should give same value
    assert(approxEqual(acc.as<Div<Meter, Mult<Second, Second>>>().value, 1.0f));

    // m/s^2 to cm/s^2 = multiply by 100
    float cm_s2 = acc.as<Div<Centi<Meter>, Mult<Second, Second>>>().value;
    assert(approxEqual(cm_s2, 100.0f));

    // Reverse using Quantity::from
    // 100 cm/s^2 = 1 m/s^2
    Acceleration from_cm_s2 =
        Acceleration::from<Div<Centi<Meter>, Mult<Second, Second>>>(100.0f);
    assert(approxEqual(from_cm_s2.value, 1.0f));
}

void units_testCompositeTypeConversions() {
    // Test Mult with different units: m * s to cm * s
    Quantity<Mult<Meter, Second>> m_s{1.0f}; // 1 meter-second
    auto cm_s = m_s.as<Mult<Centi<Meter>, Second>>();
    assert(approxEqual(cm_s.value, 100.0f));

    // Test complex composite: kg * m / s^2 (Newton)
    Force n{1.0f}; // 1 Newton
    // Verify it's kg*m/s^2
    auto asBase = n.as<Mult<Kilogram, Div<Meter, Mult<Second, Second>>>>();
    assert(approxEqual(asBase.value, 1.0f));
}

void units_testQuantityFrom() {
    // Test Quantity::from for creating quantities from different units
    Length m = Length::from<Centi<Meter>>(100.0f);
    assert(approxEqual(m.value, 1.0f));

    Length m2 = Length::from<Milli<Meter>>(1000.0f);
    assert(approxEqual(m2.value, 1.0f));

    Length m3 = Length::from<Inch>(39.3701f);
    assert(approxEqual(m3.value, 1.0f, 0.01f));

    // Test from with composite types
    Velocity ms = Velocity::from<Div<Centi<Meter>, Second>>(100.0f);
    assert(approxEqual(ms.value, 1.0f));

    // Test from with Ratio/Percent
    Level lvl = Level::from<Percent>(50.0f);
    assert(approxEqual(lvl.value, 0.5f));
}

void units_testUnitSorting() {
    // This tests that conversion works correctly even when types
    // sort differently within _Mult due to typeHash ordering

    // The hash order is: Meter < Second < Centi<Meter>
    // So Mult<Meter, Second> stays as <Meter, Second>
    // But Mult<Centi<Meter>, Second> becomes <Second, Centi<Meter>>

    Quantity<Mult<Meter, Second>> v1{1.0f};
    auto v2 = v1.as<Mult<Centi<Meter>, Second>>();

    // Should be 100 (only Meter converts to Centi<Meter>, Second stays)
    assert(approxEqual(v2.value, 100.0f));

    // Test with three elements if possible
    Quantity<Mult<Meter, Mult<Meter, Second>>> v3{1.0f}; // m^2 * s
    auto v4 = v3.as<Mult<Centi<Meter>, Mult<Centi<Meter>, Second>>>();
    // m^2 * s to cm^2 * s = 10000
    assert(approxEqual(v4.value, 10000.0f));
}

void units_testDivisionConversions() {
    // Test conversions of division types like kg/m to g/cm, kg/cm, g/m

    // 1 kg/m = 1000 g / 100 cm = 10 g/cm
    // Actually: 1 kg/m = 1000 g / 100 cm = 10 g/cm? No...
    // 1 kg = 1000 g, 1 m = 100 cm
    // So 1 kg/m = 1000 g / 100 cm = 10 g/cm? Let's verify:
    // If we have 1 kg per meter, that's 1000 g per 100 cm = 10 g/cm
    // But wait, that's the linear density. For 1 kg/m:
    // = 1000 g / 100 cm = 10 g/cm... actually that's correct!

    Quantity<Div<Kilogram, Meter>> kg_per_m{1.0f};

    // kg/m to g/m: multiply by 1000 (kg->g), denominator unchanged
    // 1 kg/m = 1000 g/m
    auto g_per_m = kg_per_m.as<Div<Kilogram, Centi<Meter>>>();
    assert(approxEqual(
        g_per_m.value,
        0.01f)); // 1 kg/m = 0.01 kg/cm = 1000 g / 100 cm = 10 g/cm...
    // Wait, let me think again:
    // 1 kg/m means 1 kg per 1 meter
    // Converting to kg/cm: 1 kg per 100 cm = 0.01 kg/cm
    assert(
        approxEqual(kg_per_m.as<Div<Kilogram, Centi<Meter>>>().value, 0.01f));

    // kg/m to g/m: 1000 g / 1 m = 1000 g/m
    // But we don't have Gram type yet... let me test with what we have

    // kg/m to kg/cm: divide by 100 (m->cm in denominator)
    // 1 kg/m = 0.01 kg/cm
    assert(
        approxEqual(kg_per_m.as<Div<Kilogram, Centi<Meter>>>().value, 0.01f));

    // Test with velocity-like quantities: m/s to cm/s (already tested but let's
    // be explicit)
    Velocity ms{1.0f}; // 1 m/s
    auto cms = ms.as<Div<Centi<Meter>, Second>>();
    assert(approxEqual(cms.value, 100.0f)); // 1 m/s = 100 cm/s

    // Test using Quantity::from
    // Create 100 cm/s and verify it equals 1 m/s
    Velocity from_cms = Velocity::from<Div<Centi<Meter>, Second>>(100.0f);
    assert(approxEqual(from_cms.value, 1.0f));
}

// ============================================================================
// OPERATOR OVERLOAD TESTS
// ============================================================================

void units_testOperatorPlus() {
    // Test operator+ (addition of same units)
    Length m1{2.0f};
    Length m2{3.0f};
    Length result = m1 + m2;
    assert(approxEqual(result.value, 5.0f));

    // Test with different values
    Time t1{10.0f};
    Time t2{20.0f};
    Time t_result = t1 + t2;
    assert(approxEqual(t_result.value, 30.0f));

    // Test with zero
    Mass kg1{5.0f};
    Mass kg2{0.0f};
    Mass m_result = kg1 + kg2;
    assert(approxEqual(m_result.value, 5.0f));

    // Test with negative values
    Length neg_m{-2.0f};
    Length pos_m{5.0f};
    Length neg_result = neg_m + pos_m;
    assert(approxEqual(neg_result.value, 3.0f));
}

void units_testOperatorMinus() {
    // Test binary operator- (subtraction of same units)
    Length m1{5.0f};
    Length m2{2.0f};
    Length result = m1 - m2;
    assert(approxEqual(result.value, 3.0f));

    // Test subtraction resulting in negative
    Length m3{2.0f};
    Length m4{5.0f};
    Length neg_result = m3 - m4;
    assert(approxEqual(neg_result.value, -3.0f));

    // Test with time
    Time t1{30.0f};
    Time t2{10.0f};
    Time t_result = t1 - t2;
    assert(approxEqual(t_result.value, 20.0f));

    // Test subtraction resulting in zero
    Mass kg1{5.0f};
    Mass kg2{5.0f};
    Mass zero_result = kg1 - kg2;
    assert(approxEqual(zero_result.value, 0.0f));
}

void units_testOperatorUnaryMinus() {
    // Test unary operator- (negation)
    Length m{5.0f};
    Length neg_m = -m;
    assert(approxEqual(neg_m.value, -5.0f));

    // Test double negation
    Length pos_m = -neg_m;
    assert(approxEqual(pos_m.value, 5.0f));

    // Test with zero
    Time t{0.0f};
    Time neg_t = -t;
    assert(approxEqual(neg_t.value, 0.0f));

    // Test with negative value
    Velocity v{-10.0f};
    Velocity neg_v = -v;
    assert(approxEqual(neg_v.value, 10.0f));
}

void units_testOperatorMultiply() {
    // Test Quantity * Quantity (creates composite units)
    // 2m * 3m = 6m^2
    Length m1{2.0f};
    Length m2{3.0f};
    auto area_result = m1 * m2;
    assert(approxEqual(area_result.value, 6.0f));
    // Verify the type is correct (Area = Quantity<Mult<Meter, Meter>>)
    static_assert(std::is_same_v<decltype(area_result), Area>);

    // Test m * m * m = m^3 (Volume)
    Length m{2.0f};
    auto vol_result = m * m * m;
    assert(approxEqual(vol_result.value, 8.0f));

    // Test velocity = length / time -> verify by multiplication
    // v = 10m / 2s = 5 m/s
    Length len{10.0f};
    Time t{2.0f};
    Velocity vel = len / t;
    assert(approxEqual(vel.value, 5.0f));

    // Test Force = Mass * Acceleration
    // F = 2kg * 3m/s^2 = 6N
    Mass mass{2.0f};
    Acceleration acc{3.0f};
    Force force = mass * acc;
    assert(approxEqual(force.value, 6.0f));

    // Test Work = Force * Length
    // W = 5N * 2m = 10J
    Force f{5.0f};
    Length d{2.0f};
    Work work = f * d;
    assert(approxEqual(work.value, 10.0f));

    // Test composite: kg * m / s = (kg*m)/s
    Mass kg{1.0f};
    Length meter{1.0f};
    Time sec{1.0f};
    auto composite = kg * meter / sec;
    assert(approxEqual(composite.value, 1.0f));
}

void units_testOperatorDivide() {
    // Test Quantity / Quantity
    // 10m / 2s = 5 m/s
    Length m{10.0f};
    Time s{2.0f};
    Velocity v = m / s;
    assert(approxEqual(v.value, 5.0f));

    // Test 6m / 3m = 2 (dimensionless)
    Length m1{6.0f};
    Length m2{3.0f};
    auto ratio = m1 / m2;
    assert(approxEqual(ratio.value, 2.0f));
    // Result should be dimensionless (Quantity<One> or similar)

    // Test acceleration = velocity / time
    // a = 10m/s / 2s = 5m/s^2
    Velocity vel{10.0f};
    Time t{2.0f};
    Acceleration acc = vel / t;
    assert(approxEqual(acc.value, 5.0f));

    // Test pressure = force / area
    // P = 10N / 2m^2 = 5Pa
    Force f{10.0f};
    Area a{2.0f};
    Pressure p = f / a;
    assert(approxEqual(p.value, 5.0f));

    // Test division with same units (creates Ratio)
    // 50% / 2 = 25%
    Quantity<Percent> pct{50.0f};
    auto half_pct = pct / 2.0f;
    assert(approxEqual(half_pct.value, 25.0f));
}

void units_testOperatorPlusEquals() {
    // Test operator+=
    Length m{5.0f};
    m += Length{3.0f};
    assert(approxEqual(m.value, 8.0f));

    // Test chaining
    Time t{10.0f};
    t += Time{5.0f};
    t += Time{2.0f};
    assert(approxEqual(t.value, 17.0f));

    // Test with negative
    Mass kg{10.0f};
    kg += Mass{-3.0f};
    assert(approxEqual(kg.value, 7.0f));

    // Test adding zero
    Velocity v{5.0f};
    v += Velocity{0.0f};
    assert(approxEqual(v.value, 5.0f));
}

void units_testOperatorMinusEquals() {
    // Test operator-=
    Length m{10.0f};
    m -= Length{3.0f};
    assert(approxEqual(m.value, 7.0f));

    // Test chaining
    Time t{20.0f};
    t -= Time{5.0f};
    t -= Time{3.0f};
    assert(approxEqual(t.value, 12.0f));

    // Test subtracting larger value (negative result)
    Mass kg{5.0f};
    kg -= Mass{8.0f};
    assert(approxEqual(kg.value, -3.0f));

    // Test subtracting zero
    Velocity v{5.0f};
    v -= Velocity{0.0f};
    assert(approxEqual(v.value, 5.0f));
}

void units_testOperatorMultiplyEquals() {
    // Test operator*= with float
    Length m{5.0f};
    m *= 2.0f;
    assert(approxEqual(m.value, 10.0f));

    // Test with fractional multiplier
    Time t{10.0f};
    t *= 0.5f;
    assert(approxEqual(t.value, 5.0f));

    // Test with double
    Mass kg{2.0f};
    kg *= 3.0;
    assert(approxEqual(kg.value, 6.0f));

    // Test with zero
    Velocity v{5.0f};
    v *= 0.0f;
    assert(approxEqual(v.value, 0.0f));

    // Test operator*= with Quantity<One>
    Length m2{4.0f};
    Quantity<One> ratio{2.5f};
    m2 *= ratio;
    assert(approxEqual(m2.value, 10.0f));
}

void units_testOperatorDivideEquals() {
    // Test operator/= with float
    Length m{10.0f};
    m /= 2.0f;
    assert(approxEqual(m.value, 5.0f));

    // Test with fractional divisor
    Time t{10.0f};
    t /= 4.0f;
    assert(approxEqual(t.value, 2.5f));

    // Test with double
    Mass kg{12.0f};
    kg /= 3.0;
    assert(approxEqual(kg.value, 4.0f));

    // Test division by same value (result = 1)
    Velocity v{5.0f};
    v /= 5.0f;
    assert(approxEqual(v.value, 1.0f));
}

void units_testOperatorComparison() {
    // Test operator==
    Length m1{5.0f};
    Length m2{5.0f};
    Length m3{3.0f};
    assert(m1 == m2);
    assert(!(m1 == m3));

    // Test operator>
    assert(m1 > m3);
    assert(!(m3 > m1));
    assert(!(m1 > m2)); // equal, not greater

    // Test operator>=
    assert(m1 >= m2); // equal
    assert(m1 >= m3); // greater
    assert(!(m3 >= m1));

    // Test operator<
    assert(m3 < m1);
    assert(!(m1 < m3));
    assert(!(m1 < m2)); // equal, not less

    // Test operator<=
    assert(m1 <= m2); // equal
    assert(m3 <= m1); // less
    assert(!(m1 <= m3));

    // Test with different unit types but same base
    Length meters{1.0f};
    Length from_cm = Length::from<Centi<Meter>>(100.0f);
    assert(meters == from_cm);

    // Test with negative values
    Length neg1{-5.0f};
    Length neg2{-10.0f};
    assert(neg1 > neg2);
    assert(neg2 < neg1);
}

void units_testScalarMultiply() {
    // Test Quantity * float
    Length m1{3.0f};
    auto result1 = m1 * 2.0f;
    assert(approxEqual(result1.value, 6.0f));

    // Test Quantity * double
    Time t{5.0f};
    auto result2 = t * 2.0;
    assert(approxEqual(result2.value, 10.0f));

    // Test with fractional scalar
    Mass kg{10.0f};
    auto result3 = kg * 0.5f;
    assert(approxEqual(result3.value, 5.0f));

    // Test with zero
    Velocity v{5.0f};
    auto result4 = v * 0.0f;
    assert(approxEqual(result4.value, 0.0f));
}

void units_testScalarDivide() {
    // Test Quantity / float
    Length m1{10.0f};
    auto result1 = m1 / 2.0f;
    assert(approxEqual(result1.value, 5.0f));

    // Test Quantity / double
    Time t{12.0f};
    auto result2 = t / 3.0;
    assert(approxEqual(result2.value, 4.0f));

    // Test with fractional result
    Mass kg{5.0f};
    auto result3 = kg / 2.0f;
    assert(approxEqual(result3.value, 2.5f));

    // Test division by 1
    Velocity v{5.0f};
    auto result4 = v / 1.0f;
    assert(approxEqual(result4.value, 5.0f));
}

void units_testFriendScalarMultiply() {
    // Test float * Quantity (friend operator)
    Length m{3.0f};
    auto result1 = 2.0f * m;
    assert(approxEqual(result1.value, 6.0f));

    // Test double * Quantity
    Time t{4.0f};
    auto result2 = 2.5 * t;
    assert(approxEqual(result2.value, 10.0f));

    // Test with zero
    Mass kg{5.0f};
    auto result3 = 0.0f * kg;
    assert(approxEqual(result3.value, 0.0f));

    // Test with one
    Velocity v{7.0f};
    auto result4 = 1.0f * v;
    assert(approxEqual(result4.value, 7.0f));
}

void units_testFriendScalarDivide() {
    // Test float / Quantity (friend operator)
    // 10.0 / 2m = 5/m (inverse meters)
    Length m{2.0f};
    auto result1 = 10.0f / m;
    assert(approxEqual(result1.value, 5.0f));
    // Type should be Quantity<Div2<One, Meter>>

    // Test double / Quantity
    Time t{4.0f};
    auto result2 = 12.0 / t;
    assert(approxEqual(result2.value, 3.0f));

    // Test with 1 / Quantity (creates inverse)
    Length m2{5.0f};
    auto inv_m = 1.0f / m2;
    assert(approxEqual(inv_m.value, 0.2f));
}

void units_testInverse() {
    // Test inverse() method
    // inverse of 5m = 0.2 1/m
    Length m{5.0f};
    auto inv_m = m.inverse();
    assert(approxEqual(inv_m.value, 0.2f));

    // Test inverse of 2s = 0.5 1/s
    Time t{2.0f};
    auto inv_t = t.inverse();
    assert(approxEqual(inv_t.value, 0.5f));

    // Test inverse of inverse
    // 1 / (1 / 5m) = 5m
    auto double_inv = inv_m.inverse();
    assert(approxEqual(double_inv.value, 5.0f));

    // Test that inverse matches 1/x
    Velocity v{10.0f};
    auto inv_v = v.inverse();
    auto one_div_v = 1.0f / v;
    assert(approxEqual(inv_v.value, one_div_v.value));
}

void units_testExplicitFloatConversion() {
    // Test explicit operator float()
    Length m{5.0f};
    float f = static_cast<float>(m);
    assert(approxEqual(f, 5.0f));

    // Test with different values
    Time t{3.14159f};
    float tf = static_cast<float>(t);
    assert(approxEqual(tf, 3.14159f));

    // Test with zero
    Mass kg{0.0f};
    float kf = static_cast<float>(kg);
    assert(approxEqual(kf, 0.0f));

    // Test with negative
    Velocity v{-7.5f};
    float vf = static_cast<float>(v);
    assert(approxEqual(vf, -7.5f));
}

void units_testComplexUnitOperations() {
    // Test complex operations with multiple units
    // (2m * 3m) / 2s = 6m^2 / 2s = 3m^2/s
    Length m1{2.0f};
    Length m2{3.0f};
    Time t{2.0f};
    auto result = (m1 * m2) / t;
    assert(approxEqual(result.value, 3.0f));

    // Test: 10m / (2s * 5s) = 10m / 10s^2 = 1 m/s^2
    Length len{10.0f};
    Time t1{2.0f};
    Time t2{5.0f};
    auto acc = len / (t1 * t2);
    assert(approxEqual(acc.value, 1.0f));

    // Test: (6m/s) * (4s) = 24m
    Velocity vel{6.0f};
    Time time{4.0f};
    auto distance = vel * time;
    assert(approxEqual(distance.value, 24.0f));

    // Test: (100m/s) / (10m/s^2) = 10s
    Velocity v{100.0f};
    Acceleration a{10.0f};
    auto time_result = v / a;
    assert(approxEqual(time_result.value, 10.0f));

    // Test: (10kg * 2m/s^2) * 5m = 100J
    Mass mass{10.0f};
    Acceleration acc2{2.0f};
    Length dist{5.0f};
    Work work = (mass * acc2) * dist;
    assert(approxEqual(work.value, 100.0f));
}

void units_testUnitSimplificationInOperations() {
    // Test that units simplify correctly in operations
    // (5m * 2s) / 2s = 5m (seconds should cancel)
    Length m{5.0f};
    Time s1{2.0f};
    Time s2{2.0f};
    auto result = (m * s1) / s2;
    // Result should be Length (meters)
    static_assert(std::is_same_v<decltype(result), Length>);
    assert(approxEqual(result.value, 5.0f));

    // Test: (10m/s) * (5s/m) = 50 (dimensionless)
    Velocity v{10.0f};
    Quantity<Div<Second, Meter>> s_per_m{5.0f};
    auto dimensionless = v * s_per_m;
    assert(approxEqual(dimensionless.value, 50.0f));

    // Test: (6m^2) / (2m) = 3m
    Area area{6.0f};
    Length len{2.0f};
    auto length_result = area / len;
    static_assert(std::is_same_v<decltype(length_result), Length>);
    assert(approxEqual(length_result.value, 3.0f));

    // Test: (12m^3) / (3m^2) = 4m
    Quantity<Mult<Meter, Meter, Meter>> vol{12.0f};
    Area area2{3.0f};
    auto len_result = vol / area2;
    assert(approxEqual(len_result.value, 4.0f));
}

void units_randomTests() {

    // converts mdps to radps
    float gyro_mult = M_PI / 180.0 / 1000.0;

    float actualRadps = 1 * gyro_mult;

    AngularVelocity quantityCast =
        AngularVelocity::from<Milli<Div<Degree, Second>>>(1.0);

    assert(approxEqual(quantityCast.value, actualRadps));

    float acc_mult = 9.80665 / 1000.0;
    float actualMps = 1 * acc_mult;

    Acceleration quantityCast2 = Acceleration::from<Milli<Gravity>>(1.0);

    assert(approxEqual(quantityCast2.value, actualMps));
}

void units_temperatureTests() {

    assert(approxEqual(Temperature::from<Celsius>(-273.15).value,
                       Temperature::from<Kelvin>(0.0).value));
    assert(approxEqual(Temperature::from<Fahrenheit>(32.0).value,
                       Temperature::from<Celsius>(0.0).value));
    assert(approxEqual(Temperature::from<Fahrenheit>(212.0).value,
                       Temperature::from<Celsius>(100.0).value));

    assert(approxEqual(Temperature::from<Fahrenheit>(-40.0).value,
                       Temperature::from<Celsius>(-40.0).value));

    assert(approxEqual(Temperature::from<Kelvin>(0.0).value,
                       Temperature::from<Celsius>(-273.15).value));
}

} // extern "C"
