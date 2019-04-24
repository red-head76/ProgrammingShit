// Copyright 2019 Luis Restat
// luis@restat.de

#include <gtest/gtest.h>
#include <vector>
#include "./Quermultiplication.h"

// _____________________________________________________________________________
TEST(Quermultiplication, divideNumber) {
  Quermultiplication QM;
  ASSERT_EQ(QM.divideNumber(234), false);
  ASSERT_EQ(QM.divideNumber(267), true);
  ASSERT_EQ(0, QM.getInputDigits(1));
  ASSERT_EQ(1, QM.getInputDigits(2));
  ASSERT_EQ(1, QM.getInputDigits(6));
  ASSERT_EQ(1, QM.getInputDigits(7));
  ASSERT_EQ(0, QM.getInputDigits(8));
  ASSERT_EQ(QM.divideNumber(235), false);
  ASSERT_EQ(QM.divideNumber(111), false);
  ASSERT_EQ(QM.divideNumber(200), false);
  ASSERT_EQ(QM.divideNumber(2777889), true);
  ASSERT_EQ(1, QM.getInputDigits(2));
  ASSERT_EQ(3, QM.getInputDigits(7));
  ASSERT_EQ(2, QM.getInputDigits(8));
  ASSERT_EQ(1, QM.getInputDigits(9));
}
TEST(Quermulitplication, digitMultiplicationStep) {
  Quermultiplication QM;
  QM.divideNumber(267);
  ASSERT_EQ(84, QM.digitMultiplicationStep());
  QM.divideNumber(2777889);
  ASSERT_EQ(395136, QM.digitMultiplicationStep());
}
TEST(Quermulitplication, digitMultiplication) {
  Quermultiplication QM;
  QM._numberToCheck = 234;
  ASSERT_EQ(2, QM.digitMultiplication());
  QM._numberToCheck = 77;
  ASSERT_EQ(4, QM.digitMultiplication());
}
TEST(Quermultiplication, process) {
  Quermultiplication QM;
  QM._nod = 2;
  QM._numberToCheck = 10;
  QM.process();
  ASSERT_EQ(4, QM._bestIterations);
  ASSERT_EQ(77, QM._bestNumber);
  QM._nod = 3;
  QM._numberToCheck = 100;
  QM.process();
  ASSERT_EQ(5, QM._bestIterations);
  ASSERT_EQ(679, QM._bestNumber);
}
