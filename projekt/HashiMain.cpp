// Copyright 2018 Luis Restat
// Using various code snippets from
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include <iostream>
#include "./Hashi.h"
#include "./Island.h"

int main(int argc, char *argv[]) {
  Hashi hashi;
  hashi.parseCommandLineArguments(argc, argv);
  // Read file and store data
  hashi.initializeHashi();
  hashi.printSolution(hashi.solvePuzzle());
  return 0;
}
