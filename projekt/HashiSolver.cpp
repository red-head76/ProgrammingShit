// Copyright 2018 Luis Restat
// Using various code snippets from
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include "./HashiSolver.h"

// _____________________________________________________________________________
HashiSolver::HashiSolver() {
}
// _____________________________________________________________________________
HashiSolver::~HashiSolver() {
}
// _____________________________________________________________________________
void HashiSolver::mainProcess(int argc, char *argv[]) {
  Hashi hashi;
  hashi.parseCommandLineArguments(argc, argv);
  hashi.initializeHashi();
  _unsolved.push_back(Hashi(hashi));
  uint variantcounter = 0;
  while (variantcounter < _unsolved.size()) {
    int state = _unsolved[variantcounter].solvePuzzle();
    switch (state) {
      // Solved
      case 0: {
        _solved.push_back(Hashi(_unsolved[variantcounter]));
        variantcounter++;
        break;
      }
        // Not solvable
      case 1: {
        // Erase the hashi, if its not the only one and theres no solution
        if (!(_solved.size() == 0 && _unsolved.size() == 1)) {
          _unsolved.erase(_unsolved.begin());
        }
        variantcounter++;
        break;
      }
        // Unsolved
      case 2: {
        _unsolved[variantcounter]._developmentStatus++;
        _unsolved[variantcounter]._tryhardmode = true;
        _unsolved.push_back(Hashi(_unsolved[variantcounter]));
        _unsolved[_unsolved.size() - 1]._variant++;
      }
      default:
        break;
      }
    }
  // Print all puzzle solved
  for (auto& hashi : _solved) {
    if (hashi._printSolutionXY) {
      hashi.printXYSolution();
    } else {
      hashi.plainSolutionSetBridges();
      hashi.printPlainSolution();
    }
  }
  if (_solved.size() == 0) {
    // set variantcounter back to its value, since it got increased to continue the
    // while loop
    variantcounter--;
    printf("%s\n", "Puzzle is not solvable!");
    _unsolved[variantcounter].plainSolutionSetBridges();
    _unsolved[variantcounter].printPlainSolution();
  }
}
// _____________________________________________________________________________
bool HashiSolver::checkEquality(Hashi hashi) {
  bool equality = false;
  for (auto& unsolvedHashi : _unsolved) {
    // Excluding the hashi to check itself
    if (&hashi != &unsolvedHashi) {
      // check _bridges.size() first for faster performance
      if (hashi._bridges.size() == unsolvedHashi._bridges.size()) {
        // than check each bridge seperately
        bool equalityCheck = true;
        for (auto& bridge : hashi._bridges) {
          if (std::count(hashi._bridges.begin(), hashi._bridges.end(), bridge)
              != std::count(unsolvedHashi._bridges.begin(),
                            unsolvedHashi._bridges.end(), bridge)) {
            equalityCheck = false;
            break;
          }
        }
        if (equalityCheck) {
          equality = true;
        }
      }
    }
  }
  return equality;
}
