// Copyright 2018 Luis Restat
// Using various code snippets from
// http://ad-wiki.informatik.uni-freiburg.de/teaching

#include "./Island.h"

// _____________________________________________________________________________
Island::Island() {}

// _____________________________________________________________________________
Island::Island(int x, int y, int N) {
  _x = x;
  _y = y;
  _requiredBridges = N;
  _healthy = true;
}
// _____________________________________________________________________________
Island::Island(const Island& island) {
  _x = island._x;
  _y = island._y;
  _requiredBridges = island._requiredBridges;
  _healthy = true;
}
// _____________________________________________________________________________
Island::~Island() {}
