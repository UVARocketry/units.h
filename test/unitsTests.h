#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void units_testPrefixCasts();

void units_testLengthConversions();
void units_testTimeConversions();
void units_testMassConversions();
void units_testAngleConversions();
void units_testRatioAndPercentConversions();
void units_testAreaConversions();
void units_testVelocityConversions();
void units_testAccelerationConversions();
void units_testCompositeTypeConversions();
void units_testQuantityFrom();
void units_testUnitSorting();
void units_testDivisionConversions();

// Operator overload tests
void units_testOperatorPlus();
void units_testOperatorMinus();
void units_testOperatorUnaryMinus();
void units_testOperatorMultiply();
void units_testOperatorDivide();
void units_testOperatorPlusEquals();
void units_testOperatorMinusEquals();
void units_testOperatorMultiplyEquals();
void units_testOperatorDivideEquals();
void units_testOperatorComparison();
void units_testScalarMultiply();
void units_testScalarDivide();
void units_testFriendScalarMultiply();
void units_testFriendScalarDivide();
void units_testInverse();
void units_testExplicitFloatConversion();
void units_testComplexUnitOperations();
void units_testUnitSimplificationInOperations();
void units_randomTests();
void units_temperatureTests();

#ifdef __cplusplus
}
#endif
