const thing = @import("header");

test "testPrefixCasts" {
    thing.units_testPrefixCasts();
}
test "testLengthConversions" {
    thing.units_testLengthConversions();
}
test "testTimeConversions" {
    thing.units_testTimeConversions();
}
test "testMassConversions" {
    thing.units_testMassConversions();
}
test "testAngleConversions" {
    thing.units_testAngleConversions();
}
test "testRatioAndPercentConversions" {
    thing.units_testRatioAndPercentConversions();
}
test "testAreaConversions" {
    thing.units_testAreaConversions();
}
test "testVelocityConversions" {
    thing.units_testVelocityConversions();
}
test "testAccelerationConversions" {
    thing.units_testAccelerationConversions();
}
test "testCompositeTypeConversions" {
    thing.units_testCompositeTypeConversions();
}
test "testQuantityFrom" {
    thing.units_testQuantityFrom();
}
test "testUnitSorting" {
    thing.units_testUnitSorting();
}
test "testDivisionConversions" {
    thing.units_testDivisionConversions();
}

// Operator overload tests
test "testOperatorPlus" {
    thing.units_testOperatorPlus();
}
test "testOperatorMinus" {
    thing.units_testOperatorMinus();
}
test "testOperatorUnaryMinus" {
    thing.units_testOperatorUnaryMinus();
}
test "testOperatorMultiply" {
    thing.units_testOperatorMultiply();
}
test "testOperatorDivide" {
    thing.units_testOperatorDivide();
}
test "testOperatorPlusEquals" {
    thing.units_testOperatorPlusEquals();
}
test "testOperatorMinusEquals" {
    thing.units_testOperatorMinusEquals();
}
test "testOperatorMultiplyEquals" {
    thing.units_testOperatorMultiplyEquals();
}
test "testOperatorDivideEquals" {
    thing.units_testOperatorDivideEquals();
}
test "testOperatorComparison" {
    thing.units_testOperatorComparison();
}
test "testScalarMultiply" {
    thing.units_testScalarMultiply();
}
test "testScalarDivide" {
    thing.units_testScalarDivide();
}
test "testFriendScalarMultiply" {
    thing.units_testFriendScalarMultiply();
}
test "testFriendScalarDivide" {
    thing.units_testFriendScalarDivide();
}
test "testInverse" {
    thing.units_testInverse();
}
test "testExplicitFloatConversion" {
    thing.units_testExplicitFloatConversion();
}
test "testComplexUnitOperations" {
    thing.units_testComplexUnitOperations();
}
test "testUnitSimplificationInOperations" {
    thing.units_testUnitSimplificationInOperations();
}
test "randomTests" {
    thing.units_randomTests();
}
test "temperatureTests" {
    thing.units_temperatureTests();
}
