// Copyright 2018 Luis Restat
// Using various code snippets from
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#ifndef HASHISOLVER_H_
#define HASHISOLVER_H_

#include <vector>
#include "./Hashi.h"

class HashiSolver {
 public:
  // Constructor
  HashiSolver();

  // Destructor
  ~HashiSolver();

  // Controls the main solving process of all hashi variants
  void mainProcess(int argc, char *argv[]);

  // Checks the equality of 2 hashis in _unsolved by comparing the _bridges
  // If 2 are equal, there is no need to calculate it twice.
  // Input: Hashi hashi
  // Return: True if 2 are equal, False otherwise
  bool checkEquality(Hashi hashi);

 private:
  std::vector<Hashi> _solved;
  std::vector<Hashi> _unsolved;
  Hashi _unsolvable;
};

#endif  // HASHISOLVER_H_
