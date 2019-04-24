// Copyright 2019 Luis Restat
// luis@restat.de

#include <getopt.h>
#include <math.h>
#include <iostream>
#include "./Quermultiplication.h"

// _____________________________________________________________________________
Quermultiplication::Quermultiplication() {
  _nod = 0;
  _numberToCheck = 0;
  _firstStep = true;
  _bestIterations = 0;
  _bestNumber = 0;
}
// _____________________________________________________________________________
void Quermultiplication::printUsageAndExit() const {
  std::cerr << "Usage: ./QuermultiplicationMain <number of digits>\n";
  exit(1);
}
// _____________________________________________________________________________
void Quermultiplication::parseCommandLineArguments(int argc, char** argv) {
  if (optind + 1 != argc) {
    printUsageAndExit();
  }

  _nod = atoi(argv[1]);
  std::cout << _nod << std::endl;
  _numberToCheck = pow(10, _nod - 1);
  _bestIterations = 0;
}
bool Quermultiplication::divideNumber(int inputNumber) {
  _inputDigits.assign(10, 0);
  int lastDigit = 0;
  while (inputNumber > 0) {
    lastDigit = inputNumber % 10;
    inputNumber = inputNumber / 10;
    // if its the first step, intercept if a 0, 1, or 5 is in _numberToCheck
    if ((lastDigit == 0 || lastDigit == 1 || lastDigit == 5) && _firstStep) {
      return false;
    }
    _inputDigits[lastDigit]++;
    // There can be only one 2, 3 or 4
    if ((_inputDigits[2] + _inputDigits[3] + _inputDigits[4]) > 1) {
      return false;
    }
  }
  return true;
}
// _____________________________________________________________________________
unsigned int Quermultiplication::digitMultiplicationStep() {
  unsigned int newNumber = 1;
  for (int i = 0; i < 10; ++i) {
    // for each digit, take the digit to the power of its occurence
    newNumber *= pow(i, _inputDigits[i]);
  }
  return newNumber;
}
// _____________________________________________________________________________
int Quermultiplication::digitMultiplication() {
  int count = 0;
  int step = _numberToCheck;
  while (step >= 10) {
    divideNumber(step);
    step = digitMultiplicationStep();
    _firstStep = false;
    count++;
  }
  return count;
}
// _____________________________________________________________________________
void Quermultiplication::printSolution() {
  printf("Best Number with %d digits: %d (%d Iteration)\n", _nod,
         _bestNumber, _bestIterations);
}
// _____________________________________________________________________________
void Quermultiplication::process() {
  while (_numberToCheck <= pow(10, _nod)) {
    _firstStep = true;
    if (!divideNumber(_numberToCheck)) {
      _numberToCheck++;
      continue;
    }
    int actualIterations = digitMultiplication();
    if (actualIterations > _bestIterations) {
      _bestNumber = _numberToCheck;
      _bestIterations = actualIterations;
    }
    _numberToCheck++;
  }
}
// _____________________________________________________________________________
int Quermultiplication::getInputDigits(int position) const {
  return _inputDigits[position];
}
