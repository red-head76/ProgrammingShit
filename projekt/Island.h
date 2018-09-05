// Copyright 2018 Luis Restat
// Using various code snippets from
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#ifndef ISLAND_H_
#define ISLAND_H_

#include <gtest/gtest.h>
#include <utility>
#include <vector>

class Island {
 public:
  // Constructors

  // Standard
  Island();

  // x, y: x-, y-Coordinates, N: Number of required Brigdes
  Island(int x, int y, int N);
  FRIEND_TEST(IslandTest, Constructor);

  // Copy constructor
  Island(const Island& island);

  // Destructor
  ~Island();

 private:
  // The x, y coordinates
  int _x;
  int _y;

  // The number of required bridges
  int _requiredBridges;

  // False, if the island is not solvable.
  // This is for better output (printing the island red)
  bool _healthy;

  // Array of the neighbours of the island
  std::vector<Island*> _neighbours;

  // Stores the amount of allowed brigdes
  // int _allowedBrigdes[4];

  friend class Hashi;

  FRIEND_TEST(HashiTest, copyConstructorTest);
  FRIEND_TEST(HashiTest, readPlainFileTest);
  FRIEND_TEST(HashiTest, readXYFileTest);
  FRIEND_TEST(HashiTest, initializeNeighboursTest);
  FRIEND_TEST(HashiTest, buildBridgeUpdateRequiredBridgesTest);
  FRIEND_TEST(HashiTest, buildBridgeUpdateNeighboursTest);
  FRIEND_TEST(HashiTest, removeFromNeighboursTest);
  FRIEND_TEST(HashiTest, checkTechnique1Test);
};

#endif  // ISLAND_H_
