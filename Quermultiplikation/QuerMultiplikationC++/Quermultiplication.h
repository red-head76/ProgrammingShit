// Copyright 2019 Luis Restat
// luis@restat.de

#ifndef QUERMULTIPLICATION_H_
#define QUERMULTIPLICATION_H_

#include <gtest/gtest.h>
#include <vector>

class Quermultiplication {
 public:
  // Constructor
  Quermultiplication();

  // Prints the usage in case of errors and exits
  void printUsageAndExit() const;

  // Parse the command line arguments
  void parseCommandLineArguments(int argc, char** argv);

  // Divides a given number into its digits
  // In this process bad digits like 0, 1, or 5 get thrown out.
  // Also 2, 3, 4 can only occur once, that means if a 2 is in the number
  // there can't be a 3, because otherwise you could write 6, etc.
  // Returns true if none of these digits are in _numberToCheck
  bool divideNumber(int inputNumber);
  FRIEND_TEST(Quermulitplikation, divideNumber);

  // Does one step for the digit multiplication
  unsigned int digitMultiplicationStep();

  // Does the whole digit multiplication and returns the count of total steps
  int digitMultiplication();
  FRIEND_TEST(Quermulitplication, digitMultiplication);

  // Prints the Solution
  void printSolution();

  // Does the whole process of searching for the smallest number with the
  // biggeest Quermulitplicationvalue
  void process();
  FRIEND_TEST(Quermultiplication, process);

  // Gets the values out of _inputDigits
  int getInputDigits(int positin) const;
  FRIEND_TEST(Quermultiplication, divideNumber);

 private:
  // Actual number to check
  unsigned int _numberToCheck;

  // Number of digits
  int _nod;

  // Vector containing the number of digits
  // [0][1][2][3][4][5][6][7][8][9]
  std::vector<int> _inputDigits;

  // Checks, if its the first step of the Quermultiplication
  bool _firstStep;

  // Best iteration value
  int _bestIterations;

  // Best number with bestIterations
  int _bestNumber;
};

#endif  // QUERMULTIPLICATION_H_
