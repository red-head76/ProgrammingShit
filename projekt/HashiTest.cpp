// Copyright 2018 Luis Restat
// Using various code snippets from
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include <gtest/gtest.h>
#include <vector>
#include "./Hashi.h"
// _____________________________________________________________________________
TEST(HashiTest, copyConstructorTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i011-n004-s04x05.xy";
  hashi.readFile(hashi._inputFileName);
  hashi.initializeNeighbours();
  Hashi hashicopy = Hashi(hashi);

  ASSERT_EQ(hashicopy._printSolutionXY, hashi._printSolutionXY);
  ASSERT_EQ(hashicopy._printPuzzle, hashi._printPuzzle);
  ASSERT_EQ(hashicopy._printInFile, hashi._printInFile);
  ASSERT_EQ(hashicopy._maxX, hashi._maxX);
  ASSERT_EQ(hashicopy._maxY, hashi._maxY);
  ASSERT_EQ(hashicopy._leftToSolve, hashi._leftToSolve);
  ASSERT_EQ(hashicopy._inputFileName, hashi._inputFileName);
  ASSERT_EQ(hashicopy._islands["00"]._requiredBridges, hashi._islands["00"]._requiredBridges);
  ASSERT_EQ(hashicopy._islands["00"]._neighbours.size(),
            hashi._islands["00"]._neighbours.size());
  for (int i = 0; i < hashi._maxX; ++i) {
    for (int j = 0; j < hashi._maxY; ++j) {
    ASSERT_EQ(hashicopy._grid[i][j], hashi._grid[i][j]);
    }
  }
  for (int i = 0; i < hashi._maxX; ++i) {
    for (int j = 0; j < hashi._maxY; ++j) {
    ASSERT_EQ(hashicopy._plainSolutionGrid[i][j], hashi._plainSolutionGrid[i][j]);
    }
  }
  ASSERT_EQ(hashicopy._bridges, hashi._bridges);
  ASSERT_EQ(hashicopy._developmentStatus, hashi._developmentStatus);
  ASSERT_EQ(hashicopy._variant, hashi._variant);
  ASSERT_EQ(hashicopy._tryhardmode, hashi._tryhardmode);
}
// _____________________________________________________________________________
TEST(HashiTest, printUsageAndExitTest) {
  Hashi hashi;
  int argc = 1;
  char* argv[1] = {
    const_cast<char*>("")
  };
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  ASSERT_DEATH(hashi.parseCommandLineArguments(argc, argv), "Usage: *");
}
// _____________________________________________________________________________
TEST(HashiTest, parseCommandLineArgumentsTest) {
  Hashi hashi;
  int argc = 4;
  char* argv[4] = {
    const_cast<char*>(""),
    const_cast<char*>("--print-puzzle"),
    const_cast<char*>("--print-in-file"),
    const_cast<char*>("parsetest")
  };
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
  hashi.parseCommandLineArguments(argc, argv);
  ASSERT_STREQ("parsetest", hashi._inputFileName);
  ASSERT_EQ(hashi._printPuzzle, true);
  ASSERT_EQ(hashi._printInFile, true);
}
// _____________________________________________________________________________
TEST(HashiTest, printFileErrorAndExit) {
  Hashi hashi;
  ASSERT_DEATH(hashi.printFileErrorAndExit("bullshitfile"),
               "Error opening file: *");
  ::testing::FLAGS_gtest_death_test_style = "threadsafe";
}
// _____________________________________________________________________________
TEST(HashiTest, readPlainFileTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i001-n002-s03x01.plain";
  hashi.readFile(hashi._inputFileName);
  // Testing setting _maxX and _maxY bounds
  ASSERT_EQ(hashi._maxX, 3);
  ASSERT_EQ(hashi._maxY, 1);
  // Testing _islands is construted correctly
  ASSERT_EQ(hashi._islands["00"]._requiredBridges, 1);
  ASSERT_EQ(hashi._islands["20"]._requiredBridges, 1);
  // Testing the size of the map, that no other islands are constructed.
  ASSERT_EQ(hashi._islands.size(), 2);
  // Testing (0,1) explicitly
  ASSERT_EQ((hashi._islands.find("01") == hashi._islands.end()), true);
  // Testing _grid
  ASSERT_EQ(hashi._grid[0][0], true);
  ASSERT_EQ(hashi._grid[1][0], false);
  ASSERT_EQ(hashi._grid[2][0], true);

  Hashi hashi2;
  hashi2._inputFileName = "./instances/i011-n004-s04x05.plain";
  hashi2.readFile(hashi2._inputFileName);

  ASSERT_EQ(hashi2._maxX, 4);
  ASSERT_EQ(hashi2._maxY, 5);
  ASSERT_EQ(hashi2._islands["00"]._requiredBridges, 2);
  ASSERT_EQ(hashi2._islands["04"]._requiredBridges, 1);
  ASSERT_EQ(hashi2._islands["30"]._requiredBridges, 3);
  ASSERT_EQ(hashi2._islands["33"]._requiredBridges, 2);
  ASSERT_EQ(hashi2._islands.size(), 4);
  ASSERT_EQ(hashi2._grid[0][0], true);
  ASSERT_EQ(hashi2._grid[1][0], false);
  ASSERT_EQ(hashi2._grid[2][0], false);
  ASSERT_EQ(hashi2._grid[3][0], true);
}
// _____________________________________________________________________________
TEST(HashiTest, readXYFileTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i001-n002-s03x01.xy";
  hashi.readFile(hashi._inputFileName);
  ASSERT_EQ(hashi._maxX, 3);
  ASSERT_EQ(hashi._maxY, 1);
  ASSERT_EQ(hashi._islands["00"]._requiredBridges, 1);
  ASSERT_EQ(hashi._islands["20"]._requiredBridges, 1);
  ASSERT_EQ(hashi._islands.size(), 2);
  ASSERT_EQ(hashi._grid[0][0], true);
  ASSERT_EQ(hashi._grid[1][0], false);
  ASSERT_EQ(hashi._grid[2][0], true);

  Hashi hashi2;
  hashi2._inputFileName = "./instances/i011-n004-s04x05.xy";
  hashi2.readFile(hashi2._inputFileName);

  ASSERT_EQ(hashi2._maxX, 4);
  ASSERT_EQ(hashi2._maxY, 5);
  ASSERT_EQ(hashi2._islands["00"]._requiredBridges, 2);
  ASSERT_EQ(hashi2._islands["04"]._requiredBridges, 1);
  ASSERT_EQ(hashi2._islands["30"]._requiredBridges, 3);
  ASSERT_EQ(hashi2._islands["33"]._requiredBridges, 2);
  ASSERT_EQ(hashi2._islands.size(), 4);
  ASSERT_EQ(hashi2._grid[0][0], true);
  ASSERT_EQ(hashi2._grid[1][0], false);
  ASSERT_EQ(hashi2._grid[2][0], false);
  ASSERT_EQ(hashi2._grid[3][0], true);
}
// _____________________________________________________________________________
TEST(HashiTest, initializeNeighboursTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i011-n004-s04x05.xy";
  hashi.readFile(hashi._inputFileName);
  hashi.initializeNeighbours();
  std::vector<Island*> v = hashi._islands["00"]._neighbours;
  // (3,0) should be a neighbour of (0,0)
  ASSERT_EQ(std::find(v.begin(), v.end(), &hashi._islands["30"]) != v.end(),
            true);
  ASSERT_EQ(hashi._islands["00"]._neighbours.size(), 2);
  ASSERT_EQ(hashi._islands["04"]._neighbours.size(), 1);
  ASSERT_EQ(hashi._islands["30"]._neighbours.size(), 2);
  ASSERT_EQ(hashi._islands["33"]._neighbours.size(), 1);
}
// _____________________________________________________________________________
TEST(HashiTest, buildBridgeGridTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i011-n004-s04x05.xy";
  hashi.readFile(hashi._inputFileName);
  // Cells between islands should be empty (false in _grid)
  // _bridges should be empty
  // testing x, direction
  ASSERT_EQ(hashi._bridges.empty(), true);
  for (int i = 1; i < 4 ; ++i) {
    ASSERT_EQ(hashi._grid[0][i], false);
  }
  // testing y direction
  for (int i = 1; i < 3 ; ++i) {
    ASSERT_EQ(hashi._grid[i][0], false);
  }
  hashi.buildBridge(&hashi._islands["00"], &hashi._islands["04"]);
  // and now filled
  for (int i = 1; i < 4 ; ++i) {
    ASSERT_EQ(hashi._grid[0][i], true);
  }
  for (int i = 1; i < 3 ; ++i) {
    ASSERT_EQ(hashi._grid[i][0], false);
  }
}
// _____________________________________________________________________________
TEST(HashiTest, buildBridgeAddToBridgesTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i011-n004-s04x05.xy";
  hashi.readFile(hashi._inputFileName);
  hashi.buildBridge(&hashi._islands["00"], &hashi._islands["04"]);
  ASSERT_EQ(hashi._bridges.size(), 1);
  std::array<int, 4> bridgecoordinates = {0, 0, 0, 4};
  ASSERT_EQ(std::count(hashi._bridges.begin(), hashi._bridges.end(),
                     bridgecoordinates), 1);
  hashi.buildBridge(&hashi._islands["00"], &hashi._islands["04"]);
  ASSERT_EQ(hashi._bridges.size(), 2);
  ASSERT_EQ(std::count(hashi._bridges.begin(), hashi._bridges.end(),
                     bridgecoordinates), 2);
}
// _____________________________________________________________________________
TEST(HashiTest, buildBridgeUpdateRequiredBridgesTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i011-n004-s04x05.xy";
  hashi.readFile(hashi._inputFileName);
  // _requiredBridges without bridges
  ASSERT_EQ(hashi._islands["00"]._requiredBridges, 2);
  ASSERT_EQ(hashi._islands["04"]._requiredBridges, 1);
  ASSERT_EQ(hashi._islands["30"]._requiredBridges, 3);
  // _leftToSolve == amount of bridges in the beginning
  ASSERT_EQ(hashi._leftToSolve, 4);
  // build a brigde between (0,0) and (0,4)
  hashi.buildBridge(&hashi._islands["00"], &hashi._islands["04"]);
  ASSERT_EQ(hashi._islands["00"]._requiredBridges, 1);
  ASSERT_EQ(hashi._islands["04"]._requiredBridges, 0);
  ASSERT_EQ(hashi._islands["30"]._requiredBridges, 3);
  // _requiredBridges of (0,4) is at 0
  ASSERT_EQ(hashi._leftToSolve, 3);
  // build a bridge between (0,0) and (3,0)
  hashi.buildBridge(&hashi._islands["00"], &hashi._islands["30"]);
  ASSERT_EQ(hashi._islands["00"]._requiredBridges, 0);
  ASSERT_EQ(hashi._islands["04"]._requiredBridges, 0);
  ASSERT_EQ(hashi._islands["30"]._requiredBridges, 2);
  ASSERT_EQ(hashi._leftToSolve, 2);
}
// _____________________________________________________________________________
TEST(HashiTest, buildBridgeUpdateNeighboursTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i080-n019-s10x09.plain";
  hashi.readFile(hashi._inputFileName);
  hashi.initializeNeighbours();
  std::vector<Island*> v = hashi._islands["16"]._neighbours;
  // (9,6) should be a neighbour of (1,6)
  ASSERT_EQ(std::find(v.begin(), v.end(), &hashi._islands["96"]) != v.end(),
            true);
  // build a bridge between (5,6) and (5,9)
  hashi.buildBridge(&hashi._islands["54"], &hashi._islands["57"]);
  // and now there is a bridge between and there should be no neighbours anymore
  ASSERT_EQ(std::find(v.begin(), v.end(), &hashi._islands["98"]) != v.end(),
             false);
}
// _____________________________________________________________________________
TEST(HashiTest, removeFromNeighboursTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i011-n004-s04x05.xy";
  hashi.readFile(hashi._inputFileName);
  hashi.initializeNeighbours();
  std::vector<Island*> v = hashi._islands["00"]._neighbours;
  // (3,0) should be a neighbour of (0,0)
  ASSERT_EQ(std::find(v.begin(), v.end(), &hashi._islands["30"]) != v.end(),
            true);
  ASSERT_EQ(hashi._islands["00"]._neighbours.size(), 2);
  hashi.removeFromNeighbours(&hashi._islands["30"]);
  ASSERT_EQ(hashi._islands["00"]._neighbours.size(), 1);
}
// _____________________________________________________________________________
TEST(HashiTest, countPossibleBridgesTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i011-n004-s04x05.xy";
  hashi.readFile(hashi._inputFileName);
  hashi.initializeNeighbours();
  ASSERT_EQ(hashi.countPossibleBridges(&hashi._islands["00"]), 3);
  ASSERT_EQ(hashi.countPossibleBridges(&hashi._islands["04"]), 2);
  ASSERT_EQ(hashi.countPossibleBridges(&hashi._islands["30"]), 4);
  ASSERT_EQ(hashi.countPossibleBridges(&hashi._islands["33"]), 2);
}
// _____________________________________________________________________________
TEST(HashiTest, countAmountOfNeighboursSizeOneTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i060-n013-s10x10.xy";
  hashi.readFile(hashi._inputFileName);
  hashi.initializeNeighbours();
  ASSERT_EQ(hashi.countAmountOfNeighboursSizeOne(&hashi._islands["00"]), 0);
  ASSERT_EQ(hashi.countAmountOfNeighboursSizeOne(&hashi._islands["23"]), 1);
  ASSERT_EQ(hashi.countAmountOfNeighboursSizeOne(&hashi._islands["52"]), 2);
}
// _____________________________________________________________________________
TEST(HashiTest, connectAllPossibleBridgesTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i011-n004-s04x05.xy";
  hashi.readFile(hashi._inputFileName);
  hashi.initializeNeighbours();
  ASSERT_EQ(hashi._bridges.size(), 0);
  // Connect 1 bridge to each neighbour of (0,0)
  hashi.connectAllPossibleBridges(&hashi._islands["00"], 1);
  ASSERT_EQ(hashi._bridges.size(), 2);
  Hashi hashi2;
  hashi2._inputFileName = "./instances/i080-n019-s10x09.plain";
  hashi2.readFile(hashi2._inputFileName);
  hashi2.initializeNeighbours();
  // Connect 2 bridges to each neighbour of (9,4)
  hashi2.connectAllPossibleBridges(&hashi2._islands["94"], 2);
  ASSERT_EQ(hashi2._bridges.size(), 6);
}
// _____________________________________________________________________________
TEST(HashiTest, checkSolvabilityTest) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i009-n004-s06x05.xy";
  hashi.readFile(hashi._inputFileName);
  hashi.initializeNeighbours();
  ASSERT_EQ(hashi.checkSolvability(), false);
}
// _____________________________________________________________________________
TEST(HashiTest, checkTechnique1Test) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i011-n004-s04x05.xy";
  hashi.readFile(hashi._inputFileName);
  hashi.initializeNeighbours();
  hashi.checkTechnique1();
  // (3,3) is the first case this occurs, because it has the lowest
  // y-coordinate of all candidates
  // Two bridges shouldve been build to (3,0)
  ASSERT_EQ(hashi._islands["33"]._requiredBridges, 0);
  ASSERT_EQ(hashi._islands["30"]._requiredBridges, 1);
  std::array<int, 4> bridgecoordinates = {3, 0, 3, 3};
  ASSERT_EQ(hashi._bridges.size(), 2);
  ASSERT_EQ(std::count(hashi._bridges.begin(), hashi._bridges.end(),
                       bridgecoordinates), 2);
  hashi.checkTechnique1();
  ASSERT_EQ(hashi._bridges.size(), 4);
  bridgecoordinates = {0, 0, 3, 0};
  ASSERT_EQ(std::count(hashi._bridges.begin(), hashi._bridges.end(),
                       bridgecoordinates), 1);
  bridgecoordinates = {0, 0, 0, 4};
  ASSERT_EQ(std::count(hashi._bridges.begin(), hashi._bridges.end(),
                       bridgecoordinates), 1);
}
// _____________________________________________________________________________
TEST(HashiTest, checkTechnique2Test) {
  Hashi hashi;
  hashi._inputFileName = "./instances/i013-n004-s07x03.plain";
  hashi.readFile(hashi._inputFileName);
  hashi.initializeNeighbours();
  hashi.checkTechnique2();
  hashi.checkTechnique2();
  hashi.checkTechnique2();
  // There should be a single bridge from (0,0) to (6,0) and (0,2)
  // and from (6,2) to (6,0) and (0,3)
  ASSERT_EQ(hashi._bridges.size(), 4);
  std::array<int, 4> bridgecoordinates = {0, 0, 6, 0};
  ASSERT_EQ(std::count(hashi._bridges.begin(), hashi._bridges.end(),
                       bridgecoordinates), 1);
  bridgecoordinates = {0, 0, 0, 2};
  ASSERT_EQ(std::count(hashi._bridges.begin(), hashi._bridges.end(),
                       bridgecoordinates), 1);
  bridgecoordinates = {0, 2, 6, 2};
  ASSERT_EQ(std::count(hashi._bridges.begin(), hashi._bridges.end(),
                       bridgecoordinates), 1);
  bridgecoordinates = {6, 0, 6, 2};
  ASSERT_EQ(std::count(hashi._bridges.begin(), hashi._bridges.end(),
                       bridgecoordinates), 1);
  Hashi hashi2;
  hashi2._inputFileName = "./instances/i060-n013-s10x10.xy";
  hashi2.readFile(hashi2._inputFileName);
  hashi2.initializeNeighbours();
  ASSERT_EQ(hashi2._bridges.size(), 0);
  // There are x bridges to build with this technique
  for (int i = 1; i <= 7; i++) { hashi2.checkTechnique2(); }
  bridgecoordinates = {5, 0, 5, 2};
  ASSERT_EQ(std::count(hashi2._bridges.begin(), hashi2._bridges.end(),
                       bridgecoordinates), 1);
  bridgecoordinates = {0, 0, 5, 0};
  ASSERT_EQ(std::count(hashi2._bridges.begin(), hashi2._bridges.end(),
                       bridgecoordinates), 1);
  bridgecoordinates = {0, 0, 0, 3};
  ASSERT_EQ(std::count(hashi2._bridges.begin(), hashi2._bridges.end(),
                       bridgecoordinates), 1);
}
