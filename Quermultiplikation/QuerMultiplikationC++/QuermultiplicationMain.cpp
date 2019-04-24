// Copyright 2019 Luis Restat
// luis@restat.de

#include "./Quermultiplication.h"

int main(int argc, char *argv[]) {
  Quermultiplication QM;
  QM.parseCommandLineArguments(argc, argv);
  QM.process();
  QM.printSolution();
}
