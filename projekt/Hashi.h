// Copyright 2018 Luis Restat
// Using various code snippets from
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#ifndef HASHI_H_
#define HASHI_H_

#include <string>
#include <vector>
#include <unordered_map>
#include "./Island.h"


class Hashi {
 public:
  // Constructor
  Hashi();

  // Copy constructor
  Hashi(const Hashi& hashi);

  // Destructor
  ~Hashi();

  // Prints the Usage
  void const printUsageAndExit();

  // Reads user input
  void parseCommandLineArguments(int argc, char** argv);

  // Prints the file error message
  void const printFileErrorAndExit(const std::string& filename);

  // Only for HashiMain.cpp
  // Prints the Solution depending on the solving state
  // Input: int solving state: 0: Solved, 1:Not solvable, 2: Unsolved
  void const printSolution(int state);

  // Prints the plain solution
  // The islands first in SetIslands because the information of requiredBridges
  // gets lost in the solving process and has to be saved first
  // Set bridges in SetBridges if the puzzle gets solved and print it than
  // with printPlainSolution finally
  void const plainSolutionSetIslands();
  void const plainSolutionSetBridges();
  void const printPlainSolution();

  // Prints the xy solution
  void const printXYSolution();

  // Reads a .plain or .xy file to be resolved
  void readFile(const std::string& filename);

  // debugging
  void debug();
  bool _debug;

  // Initializes the hashi
  void initializeHashi();

  // updates all possible neighbours for one island
  void initializeNeighbours();

  // builds bridges
  // Sets the _grid to true if a bridge is builded at this cell
  // Adds the island coordinates to _bridges
  // Updates the _requiredBridges value of the connected islands
  // and updates _lefToSolve in case _requiredBridges == 0
  // Updates the neighbours perpendicular to the bridge
  void buildBridge(Island* const islandptr1, Island* const islandptr2);

  // Subfunction of buildBridge
  // If an island gets to _requiredBridges=0, remove it from its neighbours
  // _neighbours
  void removeFromNeighbours(const Island* islandptr);

  // Subfunction of buildBridge
  // After a bridge is builded check if any neighbours must be deleted,
  // because a bride seperates 2 islands.
  // (x, y) are the coordinates of a part of a builded bridge
  // 'h' for horizontal bridges, 'v' for vertical ones
  void updateNeighbours(const int x, const int y, const char c);

  // Subfunction of updateNeighbours
  // Checks if there is an island at the given coordinates
  // Input: the x and y coordinate one wants to check
  // Return: the adress of the island if its an island,
  //         nullptr otherwise
  Island* const checkForIsland(const int x, const int y);

  // Functions for checking the next true cell in _grid in the
  // respective direction
  int const checkGridUp(const int x, const int y);
  int const checkGridDown(const int x, const int y);
  int const checkGridLeft(const int x, const int y);
  int const checkGridRight(const int x, const int y);

  // Solves the puzzle by looping over different solving techniques
  // Returns a number corresponsding to the status of the puzzle
  // 0: Solved
  // 1: Not Solvable
  // 2: Unsolved
  int solvePuzzle();

  // Counts the possible bridges by adding all _requiredBridges from neighbours
  // Input: Island*
  // Return: the number of possible Bridges
  int const countPossibleBridges(Island* const islandptr);

  // Counts the amount of neighbours with size one
  // Input: Island*
  // Return: the number of neighbours with _requiredBridges = 1
  int const countAmountOfNeighboursSizeOne(Island* const islandptr);

  // Connect all possible Bridges
  // Input: The island the pointer to the island the bridges should be connected
  //        and the max amount of connections per bridge (1 or 2)
  void connectAllPossibleBridges(Island* islands, int max);

  // Checks if a puzzle is solvable
  // if (number of required bridges > number of possible bridges)
  // Input: void
  // Return: True if the puzzle is solvable, false if its not solvable
  bool checkSolvability();

  // Various solving techniques solvePuzzle uses
  void checkTechnique1();
  // if (amount of possible bridges == amount of required bridges) {
  //   connect all; }

  void checkTechnique2();
  // if (amount of required bridges - amount of neighbours with capacity=1
  //    == (amount of neighbours with capacity > 1) - 1) {
  //   connect one bridge to all neighbours with capacity > 1;

  // void checkTechnique3()
  // check for island isolation;

  // Chooses to build a bridge if none other solving technique is succesfull
  void tryhard();

 private:
  bool _printSolutionXY;
  bool _printPuzzle;
  bool _printInFile;

  int _maxX;
  int _maxY;
  // At the start the amount of bridges. As a island gets 0 at
  // _requiredBridges it gets reduced by 1. If its at 0, the puzzle is solved
  // ATTENTION: x and y are inverted here to be able to print lines at the end
  int _leftToSolve;
  const char* _inputFileName;

  // Stores the islands of the given puzzle
  std::unordered_map<std::string, Island>  _islands;

  // FOR OPTIONAL:
  // Store island combinations for isolation checking
  // First vector for different isolated pathes,
  // Second vector for islands contained in this path
  // std::vector<std::vector<Island*> > _combinations;

  // Store empty/filled cells to check if building a bridge is valid
  // or to get the next neighbour
  // x: starts with 0, horizontally
  // y: starts with 0, vertically
  // filled == true, empty == false
  bool** _grid;

  // Grid for printing the plain solution at the end
  char** _plainSolutionGrid;

  // Stores the builded bridges with coordinates x1, y1, x2, y2
  std::vector<std::array<int, 4> > _bridges;

  // Checks, if any solving technique was succesfull
  bool _anythingChanged;

  // FOR OPTIONAL: FIND ALL SOLUTIONS
  // Development status. If the puzzle has many solutions the program has
  // to guess a bridge
  // the first guess has development status 0. If more guesses are required,
  // development status increases
  int _developmentStatus;

  // At each Developmentstatus there could be some variants.
  // _variant refers which variant of the DevelopmentStatus it is.
  int _variant;

  // If no solution can be found with the normal techniques, this builds
  // a random bridge in tryhard(). Gets activated in HashiSolverMain.cpp.
  bool _tryhardmode;

  FRIEND_TEST(HashiTest, copyConstructorTest);
  FRIEND_TEST(HashiTest, parseCommandLineArgumentsTest);
  FRIEND_TEST(HashiTest, readPlainFileTest);
  FRIEND_TEST(HashiTest, readXYFileTest);
  FRIEND_TEST(HashiTest, buildBridgeGridTest);
  FRIEND_TEST(HashiTest, buildBridgeAddToBridgesTest);
  FRIEND_TEST(HashiTest, buildBridgeUpdateRequiredBridgesTest);
  FRIEND_TEST(HashiTest, buildBridgeUpdateNeighboursTest);
  FRIEND_TEST(HashiTest, removeFromNeighboursTest);
  FRIEND_TEST(HashiTest, initializeNeighboursTest);
  FRIEND_TEST(HashiTest, countPossibleBridgesTest);
  FRIEND_TEST(HashiTest, countAmountOfNeighboursSizeOneTest);
  FRIEND_TEST(HashiTest, connectAllPossibleBridgesTest);
  FRIEND_TEST(HashiTest, checkSolvabilityTest);
  FRIEND_TEST(HashiTest, checkTechnique1Test);
  FRIEND_TEST(HashiTest, checkTechnique2Test);
  FRIEND_TEST(HashiTest, checkTechnique3Test);
  friend class HashiSolver;
};

#endif  // HASHI_H_

// sortxy-Test schreiben
// HashiSolver
// checkTechnique2-test schreiben
// countamountofneighbourssizeone-test schreiben
// i060 check6 errror at (0,5)
