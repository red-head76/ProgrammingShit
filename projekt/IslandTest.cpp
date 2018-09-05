// Copyright 2018 Luis Restat
// Using various code snippets from
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include <gtest/gtest.h>
#include "./Island.h"

// _____________________________________________________________________________
TEST(IslandTest, Constructor) {
  Island i = Island(0, 1, 10);
  ASSERT_EQ(i._x, 0);
  ASSERT_EQ(i._y, 1);
  ASSERT_EQ(i._requiredBridges, 10);
}
