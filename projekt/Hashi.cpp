// Copyright 2018 Luis Restat
// Using various code snippets from
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include <getopt.h>
#include <stdio.h>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include "./Island.h"
#include "./Hashi.h"

// _____________________________________________________________________________
Hashi::Hashi() {
  _grid = nullptr;
  _plainSolutionGrid = nullptr;
  _leftToSolve = 0;
  _developmentStatus = 0;
  _variant = 0;
  _tryhardmode = false;
}
Hashi::Hashi(const Hashi& hashi) {
  _printSolutionXY = hashi._printSolutionXY;
  _printPuzzle = hashi._printPuzzle;
  _printInFile = hashi._printInFile;
  _maxX = hashi._maxX;
  _maxY = hashi._maxY;
  _leftToSolve = hashi._leftToSolve;
  _inputFileName = hashi._inputFileName;
  std::ostringstream oss;
  for (auto& oldisland : hashi._islands) {
    oss.str("");
    oss << oldisland.second._x << oldisland.second._y;
    _islands[oss.str()] = Island(oldisland.second);
  }
  // initialize neighbours new, because its an vector of pointers
  // for each island in the new copy add all addresses from the islands with
  // the same coordinates as the neighbours from the old hashi
  for (auto& island : _islands) {
    island.second._neighbours.clear();
    oss.str("");
  }
  for (auto& oldisland : hashi._islands) {
    for (auto& neighbour : oldisland.second._neighbours) {
      oss.str("");
      oss << oldisland.second._x << oldisland.second._y;
      std::string islandstr = oss.str();
      oss.str("");
      oss << neighbour->_x << neighbour->_y;
      std::string neighbourstr = oss.str();
      _islands[islandstr]._neighbours.push_back(&_islands[neighbourstr]);
    }    
  }
  _grid = new bool*[hashi._maxX];  
  for (int i = 0; i < hashi._maxX; i++) {
    _grid[i] = new bool[hashi._maxY];
  }
  for (int i = 0; i < hashi._maxX; ++i) {
    for (int j = 0; j < hashi._maxY; ++j) {
      _grid[i][j] = hashi._grid[i][j];
    }
  }
  _plainSolutionGrid = new char*[hashi._maxY];
  for (int i = 0; i < hashi._maxY; i++) {
    _plainSolutionGrid[i] = new char[hashi._maxX];
  }
  for (int i = 0; i < hashi._maxY; ++i) {
    for (int j = 0; j < hashi._maxX; ++j) {
      _plainSolutionGrid[i][j] = hashi._plainSolutionGrid[i][j];
    }
  }
  _bridges = hashi._bridges;
  _developmentStatus = hashi._developmentStatus;
  _variant = hashi._variant;
  _tryhardmode = hashi._tryhardmode;
  _debug = hashi._debug;
}
// _____________________________________________________________________________
Hashi::~Hashi() {
  // deleting _grid
  if (_grid != nullptr) {
    for (int i = 0; i < _maxX; ++i) {
      delete[] _grid[i];
    }
    delete[] _grid;
  }
  // deleting _plainSolutionGrid
  if (_plainSolutionGrid != nullptr) {
    for (int i = 0; i < _maxY; ++i) {
      delete[] _plainSolutionGrid[i];
    }
    delete[] _plainSolutionGrid;
  }
}

// _____________________________________________________________________________
void Hashi::parseCommandLineArguments(int argc, char** argv) {
  struct option options[] = {
    {"print-solution-xy", 0, NULL, 'x'},
    {"print-puzzle", 0, NULL, 'p'},
    {"print-in-file", 0, NULL, 'f'},
    {"debug", 0, NULL, 'd'},
    {NULL, 0, NULL, 0}
  };
  optind = 1;

  // Default values
  _printSolutionXY = false;
  _printPuzzle = false;
  _inputFileName = "";
  _printInFile = false;
  _debug = false;

  while (true) {
    char c = getopt_long(argc, argv, "xpfd", options, NULL);
    if (c == -1) { break; }
    switch (c) {
      case 'x':
        _printSolutionXY = true;
        break;
      case 'p':
        _printPuzzle = true;
        break;
      case 'f':
        _printInFile = true;
        break;
      case 'd':
        _debug = true;
        break;
      case '?':
      default:
        printUsageAndExit();
    }
  }

  // input file is required argument.
  if (optind + 1 != argc) {
    printUsageAndExit();
  }
  _inputFileName = argv[optind];
}
// _____________________________________________________________________________
void const Hashi::printUsageAndExit() {
  fprintf(stderr, "Usage: ./HashiMain [options] <filename>\n");
  fprintf(stderr, "Options:\n");
  fprintf(stderr, "--print-solution-xy (-x) [bool]:\n");
  fprintf(stderr, "Print the solution of the puzzle in xy-coordinates format.");
  fprintf(stderr, " If false, it prints it in plain format (default: false)\n");
  fprintf(stderr, "--print-puzzle (-p) [bool]:\n");
  fprintf(stderr, "Print the puzzle to solve (default: false)\n");
  fprintf(stderr, "--print-in-file (-f) [bool]:\n");
  fprintf(stderr, "Print the solution in a file (default: false)\n");
  exit(1);
}
// _____________________________________________________________________________
void const Hashi::printFileErrorAndExit(const std::string& filename) {
  std::cerr << "Error opening file: " << filename << std::endl;
  std::cerr << "First line should be look like this:" << std::endl;
  std::cerr << "# X:Y (plain) or # X:Y (xy)" << std::endl;
  exit(1);
}
// _____________________________________________________________________________
void const Hashi::printSolution(int state) {
  switch (state) {
    case 0: {
      // Puzzle solved
      if (_printSolutionXY) {
        printXYSolution();
      } else {
        plainSolutionSetBridges();
        printPlainSolution();
      }
      break;
    }
    case 1: {
      // Not solvable
      if (_leftToSolve != 0) {
        printf("%s\n", "Puzzle is not solvable!");
        plainSolutionSetBridges();
        printPlainSolution();
      }
      break;
    }
    case 2: {
      printf("%s\n", "Puzzle is not completely solved!");
      // Unsolved
      plainSolutionSetBridges();
      printPlainSolution();
    }
    default:
      break;
  }
}
// _____________________________________________________________________________
void const Hashi::plainSolutionSetIslands() {
  _plainSolutionGrid = new char*[_maxY];
  for (int i = 0; i < _maxY; i++) {
    _plainSolutionGrid[i] = new char[_maxX];
  }
  for (int i = 0; i < _maxY; ++i) {
    for (int j = 0; j < _maxX; ++j) {
      _plainSolutionGrid[i][j] = ' ';
    }
  }
  for (auto & island : _islands) {
    // converting int (digit) to char
    char c = '0' + island.second._requiredBridges;
    _plainSolutionGrid[island.second._y][island.second._x] = c;
  }
}
// _____________________________________________________________________________
void const Hashi::plainSolutionSetBridges() {
  for (auto & bridge : _bridges) {
    int bridgeamount = std::count(_bridges.begin(), _bridges.end(), bridge);
    // compare x-values to know if its an horizontal or vertical bridge
    // BUT ATTENTION: _plainsolutiongrid is inverted!
    if (bridge[0] == bridge[2]) {
      // vertical in _grid
      if (bridgeamount == 1) {
        for (int i = bridge[1] + 1; i < bridge[3]; ++i) {
          _plainSolutionGrid[i][bridge[0]] = '|';
        }
      } else {
        for (int i  = bridge[1] + 1; i < bridge[3]; ++i) {
          _plainSolutionGrid[i][bridge[0]] = 'H';
        }
      }
    } else {
      // horizontal in _grid
      if (bridgeamount == 1) {
        for (int i = bridge[0] + 1; i < bridge[2]; ++i) {
          _plainSolutionGrid[bridge[1]][i] = '-';
        }
      } else {
        for (int i = bridge[0] + 1; i < bridge[2]; ++i) {
          _plainSolutionGrid[bridge[1]][i] = '=';
        }
      }
    }
  }
}
// _____________________________________________________________________________
void const Hashi::printPlainSolution() {
  printf("%s\n", "# (plain.solution)");
  for (int i = 0; i < _maxY; ++i) {
    for (int j = 0; j < _maxX; ++j) {
      Island* islandptr = checkForIsland(j, i);
      // If the island isnt healthy (solvable), print it red
      if (islandptr != nullptr && !islandptr->_healthy) {
        printf("\x1b[31m" "%c" "\x1b[0m", _plainSolutionGrid[i][j]);
      } else {
        printf("%c", _plainSolutionGrid[i][j]);
      }
    }
    printf("\n");
  }
}
// _____________________________________________________________________________
void const Hashi::printXYSolution() {
  printf("%s\n", "# (xy.solution)");
  printf("%s\n", "# x1,y1,x2,y2");
  for (auto & bridge : _bridges) {
    printf("%d,%d,%d,%d\n", bridge[0], bridge[1], bridge[2], bridge[3]);
  }
}
// _____________________________________________________________________________
void Hashi::readFile(const std::string& filename) {
  std::ifstream file(filename.c_str());
  if (!file.is_open()) { printFileErrorAndExit(filename); }
  _islands.clear();
  std::string line;
  // the numbers for x- and y-dimensions of the puzzle and
  // check if its a .plain (true) or a .xy (false) file
  // write first x-dimension (0), y-dimension (1), plain or xy (2)
  std::vector<std::string> words;
  std::string word;
  bool plain;
  size_t wordCount = 0;
  _maxX = 0;
  _maxY = 0;

  // Read the size and type of data from first line
  // and print error message, if data is not formatted correctly
  std::getline(file, line);
  for (uint i = 0; i < line.length(); ++i) {
    char c = line[i];
    if ('0' <= c && c <= '9') {
      word.push_back(c);
    } else if ('a' <= c && c <= 'z') {
      word.push_back(c);
    } else {
      if (word.length() > 0) {
        words.push_back(word);
        word = "";
        wordCount++;
      }
    }
  }
  // Choose between .xy and .plain file and throw an error if none is the case
  if (words[2] == "plain") {
    plain = true;
  } else if (words[2] == "xy") {
    plain = false;
  } else { printFileErrorAndExit(filename); }

  // Set the bounds of the puzzle
  _maxX = std::stoi(words[0]);
  _maxY = std::stoi(words[1]);
  if (_maxX == 0 || _maxY == 0) {
    printFileErrorAndExit(filename);
  }
  // Initializing 2 dimensionsional array _grid
  _grid = new bool*[_maxX];
  for (int i = 0; i < _maxX; i++) {
    _grid[i] = new bool[_maxY];
  }
  for (int i = 0; i < _maxX; ++i) {
    for (int j = 0; j < _maxY; ++j) {
      _grid[i][j] = false;
    }
  }

  words.clear();

  // Read .plain file
  if (plain) {
    // Stores the x-, y-coordinates and the number of required bridges
    int lineCount = 0;
    std::ostringstream oss;
    while (true) {
      std::getline(file, line);
      if (file.eof()) { break; }
      for (uint i = 0; i < line.length(); i++) {
        // Clear oss for every new Island
        // i ^= x-coordinate, lineCount ^= y-coordinate
        char c = line[i];
        if ('0' <= c && c <= '9') {
          oss.str("");
          oss << i << lineCount;
          _islands[oss.str()] =
              Island(i, lineCount, std::stoi(std::string(1, c)));
          _leftToSolve++;
        }
      }
      lineCount++;
    }
  }

  // Read .xy file
  if (!plain) {
    std::ostringstream oss;
    while (true) {
      std::getline(file, line);
      if (file.eof()) { break; }
      oss.str("");
      oss << line[0] << line[2];
      _islands[oss.str()] = Island(std::stoi(std::string(1, line[0])),
                                   std::stoi(std::string(1, line[2])),
                                   std::stoi(std::string(1, line[4])));
      _leftToSolve++;
    }
  }
  for (auto const& island : _islands) {
    _grid[island.second._x][island.second._y] = true;
  }
}
// _____________________________________________________________________________
void Hashi::debug() {
  for (auto & island : _islands) {
    // converting int (digit) to char
    char c = '0' + island.second._requiredBridges;
    _plainSolutionGrid[island.second._y][island.second._x] = c;
  }
  plainSolutionSetBridges();
  printPlainSolution();
  printf("\n");
}
// _____________________________________________________________________________
void Hashi::initializeHashi() {
  readFile(_inputFileName);
  if (!_printSolutionXY) {
    plainSolutionSetIslands();
  }
  initializeNeighbours();
}
// _____________________________________________________________________________
void Hashi::initializeNeighbours() {
  for (auto& island : _islands) {
    // Check for neighbours in all directions
    int& x = island.second._x;
    int& y = island.second._y;
    int i;
    Island* islandptr;
    // up
    i = checkGridUp(x, y);
    islandptr = checkForIsland(x, y - i);
    if (islandptr != nullptr) {
      island.second._neighbours.push_back(islandptr);
    }
    // down
    i = checkGridDown(x, y);
    islandptr = checkForIsland(x, y + i);
    if (islandptr != nullptr) {
      island.second._neighbours.push_back(islandptr);
    }
    // left
    i = checkGridLeft(x, y);
    islandptr = checkForIsland(x - i, y);
    if (islandptr != nullptr) {
      island.second._neighbours.push_back(islandptr);
    }
    // right
    i = checkGridRight(x, y);
    islandptr = checkForIsland(x + i, y);
    if (islandptr != nullptr) {
      island.second._neighbours.push_back(islandptr);
    }
  }
}
// _____________________________________________________________________________
void Hashi::buildBridge(Island* const islandptr1, Island* const islandptr2) {
  int x1 = islandptr1->_x;
  int x2 = islandptr2->_x;
  int y1 = islandptr1->_y;
  int y2 = islandptr2->_y;
  //  printf("{%d, %d, %d, %d}", x1, y1, x2, y2);
  // Remove 1 from _requiredBridges from each island the bridge connects
  islandptr1->_requiredBridges--;
  if (islandptr1->_requiredBridges == 0) {
    _leftToSolve--;
    removeFromNeighbours(islandptr1);
  }
  islandptr2->_requiredBridges--;
  if (islandptr2->_requiredBridges == 0) {
    _leftToSolve--;
    removeFromNeighbours(islandptr2);
  }
  // x2 should always be greater than x1. Switch if its not the case.
  if (x2 < x1) {
    int temp = x1;
    x1 = x2;
    x2 = temp;
  }
  // Same for y
  if (y2 < y1) {
    int temp = y1;
    y1 = y2;
    y2 = temp;
  }
  // Sets the _grid to true if a brigde is builded at this cell
  // and updates the neighbours perpendicular to the bridge
  if (x1 != x2) {
    for (int i = x1 + 1; i < x2; ++i) {
      _grid[i][y1] = true;
      updateNeighbours(i, y1, 'h');
    }
  }
  if (y1 != y2) {
    for (int i = y1 + 1; i < y2; ++i) {
      _grid[x1][i] = true;
      updateNeighbours(x1, i, 'v');
    }
  }
  // Adds the island coordinates to _bridges
  // x1, y1, x2, y2 are sorted by size, so it cant be doubled
  std::array<int, 4> bridgecoordinates = {x1, y1, x2, y2};
  _bridges.push_back(bridgecoordinates);
}
// _____________________________________________________________________________
void Hashi::removeFromNeighbours(const Island* islandptr) {
  for (auto & neighbour : islandptr->_neighbours) {
    for (auto & neighboursNeighbour : neighbour->_neighbours) {
      if (neighboursNeighbour == islandptr) {
        neighbour->_neighbours.erase(
            std::remove(neighbour->_neighbours.begin(),
                        neighbour->_neighbours.end(), islandptr),
            neighbour->_neighbours.end());
      }
    }
  }
}
// _____________________________________________________________________________
void Hashi::updateNeighbours(const int x, const int y, const char c) {
  // if a horizontal bridge is build, check up and down at each part of
  // the bridge if a Island is the next true cell. If its the case,
  // delete _neighbours from each other. If not, break. Vertical equally.
  int i;
  Island* islandptr1;
  Island* islandptr2;
  std::ostringstream oss;
  if (c == 'h') {
    i = checkGridUp(x, y);
    islandptr1 = checkForIsland(x, y - i);
    i = checkGridDown(x, y);
    islandptr2 = checkForIsland(x, y + i);
    if (islandptr1 != nullptr && islandptr2 != nullptr) {
      islandptr1->_neighbours.erase(std::remove(islandptr1->_neighbours.begin(),
      islandptr1->_neighbours.end(), islandptr2),
                                    islandptr1->_neighbours.end());
      islandptr2->_neighbours.erase(std::remove(islandptr2->_neighbours.begin(),
      islandptr2->_neighbours.end(), islandptr1),
                                    islandptr2->_neighbours.end());
    }
  }
  if (c == 'v') {
    i = checkGridLeft(x, y);
    islandptr1 = checkForIsland(x - i, y);
    i = checkGridRight(x, y);
    islandptr2 = checkForIsland(x + i, y);
    if (islandptr1 != nullptr && islandptr2 != nullptr) {
      islandptr1->_neighbours.erase(std::remove(islandptr1->_neighbours.begin(),
      islandptr1->_neighbours.end(), islandptr2),
                                    islandptr1->_neighbours.end());
      islandptr2->_neighbours.erase(std::remove(islandptr2->_neighbours.begin(),
      islandptr2->_neighbours.end(), islandptr1),
                                    islandptr2->_neighbours.end());
    }
  }
}
// _____________________________________________________________________________
Island* const Hashi::checkForIsland(const int x, const int y) {
  std::ostringstream oss;
  oss << x << y;
  if (_islands.find(oss.str()) != _islands.end()) {
    return &_islands[oss.str()];
  } else {
    return nullptr;
  }
}
// _____________________________________________________________________________
int const Hashi::checkGridUp(const int x, const int y) {
  // start with one step ahead, not at the island itself
  int i = 1;
  if (y > 0) {
    while (!_grid[x][y - i] && y - i > 0) {
      ++i;
    }
  }
  return i;
}
int const Hashi::checkGridDown(const int x, const int y) {
  // here +1 in the condition, because _grid[x][y + 1] must exist
  int i = 1;
  if (y + 1 < _maxY) {
    while (!_grid[x][y + i] && y + (i + 1) < _maxY) {
      ++i;
    }
  }
  return i;
}
int const Hashi::checkGridLeft(const int x, const int y) {
  int i = 1;
  if (x > 0) {
    while (!_grid[x - i][y] && x - i > 0) {
      ++i;
    }
  }
  return i;
}
int const Hashi::checkGridRight(const int x, const int y) {
  int i = 1;
  if (x + 1 < _maxX) {
    while (!_grid[x + i][y] && x + (i + 1) < _maxX) {
      ++i;
    }
  }
  return i;
}
// _____________________________________________________________________________
int Hashi::solvePuzzle() {
  _anythingChanged = true;
  while (_anythingChanged) {
    if (_debug) { debug(); }
    _anythingChanged = false;
    // Check if the puzzle is solved
    if (_leftToSolve == 0) { return 0; }
    // Check if the puzzle is solvable
    if (!checkSolvability()) { return 1; }
    // Always continue, if a technique was succesfull to test the easiest
    // techniques first after each step.
    checkTechnique1();
    if (_anythingChanged) { continue; }
    checkTechnique2();
    if (_anythingChanged) { continue; }
    tryhard();
  }
  return 2;
}
// _____________________________________________________________________________
int const Hashi::countPossibleBridges(Island* const islandptr) {
  int possibleBr = 0;
  for (auto& neighbour : islandptr->_neighbours) {
    // Only double Bridges are allowed
    possibleBr = possibleBr +
                 std::min(neighbour->_requiredBridges, 2);
  }
  return possibleBr;
}
// _____________________________________________________________________________
int const Hashi::countAmountOfNeighboursSizeOne(Island *const islandptr) {
  int counter = 0;
  for (auto& neighbour : islandptr->_neighbours) {
    if (neighbour->_requiredBridges == 1) {
      counter++;
    }
  }
  return counter;
}
// _____________________________________________________________________________
void Hashi::connectAllPossibleBridges(Island *island, int max) {
  // To decrementing for loop to prevent the case that neighbour.size() = 3
  // and neighbour 1 is erased. (neighbour gets at the end through erasing)
  // Than this neighbour is called twice.
  for (int j = island->_neighbours.size() - 1; j >= 0; j--) {
    Island* neighbour = island->_neighbours[j];
    int requiredBridges = neighbour->_requiredBridges;
    for (int i = 1; i <= std::min(requiredBridges, max); i++) {
      buildBridge(island, neighbour);
    }
  }
}
// _____________________________________________________________________________
bool Hashi::checkSolvability() {
  for (auto& island : _islands) {
    if (island.second._requiredBridges > countPossibleBridges(&island.second)) {
      island.second._healthy = false;
      return false;
    }
  }
  return true;
}
// _____________________________________________________________________________
void Hashi::checkTechnique1() {
  for (auto& island : _islands) {
    if (countPossibleBridges(&island.second) ==
        island.second._requiredBridges && island.second._requiredBridges != 0) {
      connectAllPossibleBridges(&island.second, 2);
      _anythingChanged = true;
      break;
    }
  }
}
// _____________________________________________________________________________
// void Hashi::checkTechnique3() {
//   check for island isolation;
//   block bridges in _allowedBridges;
// }
// _____________________________________________________________________________
void Hashi::checkTechnique2() {
  for (auto& island : _islands) {
    int singleNeighbours = countAmountOfNeighboursSizeOne(&island.second);
    int multipleNeighbours =
        island.second._neighbours.size() - singleNeighbours;
    if (island.second._requiredBridges - singleNeighbours ==
        2 * multipleNeighbours - 1 && island.second._requiredBridges > 0) {
      for (auto& neighbour : island.second._neighbours) {
        if (neighbour->_requiredBridges > 1) {
          buildBridge(neighbour, &island.second);
          _anythingChanged = true;
        }
      }
      if (_anythingChanged) {
        break;
      }
    }
  }
}
// _____________________________________________________________________________
void Hashi::tryhard() {
  // if a bridge is buildable: build it
  if (_tryhardmode) {
    int tries = 0;
    for (auto& island : _islands) {
      if (countPossibleBridges(&island.second) > 0 &&
          island.second._requiredBridges > 0) {
        while(tries <= _variant) {
          if (tries == _variant) {
            // one bridge to one of the neighbours must be right
            buildBridge(&island.second, island.second._neighbours[_variant]);
            _anythingChanged = true;
          }
          tries++;
        }
      }
      break;
    }
  }
}
