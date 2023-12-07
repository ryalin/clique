#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <unistd.h>
#include <iomanip>

#include "test.h"
#include "sequential/sequential.h"
#include "parallel/parallel.h"
#include "timing.h"
#include "util.h"

// Generates graphs then runs benchmarks on sequential and parallel versions
int main(int argc, char *argv[]) {

  // Input parameters:
  // c: trigger correctness (default checks with 120 clique size)
  // t: target clique size (to search for)

  if (argc == 1) {
    std::cout << "Not enough arguments" << std::endl;
    displayUsage();
    return 1;
  }

  int option, t;
  bool checkCorrectness = false;
  while ((option = getopt(argc, argv, "ct:h")) != -1) {
    switch (option) {
      case 'c':
        checkCorrectness = true;
        t = 120;
        break;
      case 't':
        t = atoi(optarg);
        break;
      case 'h':
        displayUsage();
        return 1;
      case '?':
        displayUsage();
        return 1;
      default:
        std::cerr << "Unknown option: " << static_cast<char>(optopt) << std::endl;
        displayUsage();
        return 1;
    }
  }

  std::vector<Graph> tests;
  std::cout << "\nReading graphs..." << std::flush;
  // One clique graphs with size n, and total size 2n
  tests.push_back({"one clique 50", readGraphFromTxt("tests/oneClique50.txt")});
  tests.push_back({"one clique 500", readGraphFromTxt("tests/oneClique500.txt")});
  tests.push_back({"one clique 5000", readGraphFromTxt("tests/oneClique5000.txt")});

  // Multiple cliques with max size of n
  tests.push_back({"multiple cliques 50", readGraphFromTxt("tests/multiClique50.txt")});
  tests.push_back({"multiple cliques 500", readGraphFromTxt("tests/multiClique500.txt")});
  tests.push_back({"multiple cliques 5000", readGraphFromTxt("tests/multiClique5000.txt")});

  // Random graph of n size
  tests.push_back({"random graph 50", readGraphFromTxt("tests/randomGraph50.txt")});
  tests.push_back({"random graph 500", readGraphFromTxt("tests/randomGraph500.txt")});
  tests.push_back({"random graph 5000", readGraphFromTxt("tests/randomGraph5000.txt")});
  std::cout << "done\n" << std::endl;

  if (checkCorrectness) { // Correctness toggle
    std::cout << "=============== Correctness Test ================" << std::endl;
    for (auto graph: tests) {
      std::cout << "Running test " << graph.name << std::endl; 
      bool nonRecurse = sequentialClique(graph.nodes, t);
      bool recurse = sequentialRecursive(graph.nodes, t);
      if (nonRecurse != recurse) {
        std::cout << "Correctness Check Failed" << std::endl;
      }
    }
    std::cout << "Test passed" << std::endl;

  } else { // Benchmarking
    std::cout << "=============== Benchmark Test ================" << std::endl;
    std::vector<double> sequentialTimes;
    std::vector<double> parallelOMPTimes;
    std::vector<double> parallelCUDATimes;

    for (auto graph: tests) {
      std::cout << "Running test " << graph.name << std::endl; 

      // Sequential
      Timer sequentialTimer;
      sequentialClique(graph.nodes, t);
      double simTime = sequentialTimer.elapsed();
      sequentialTimes.push_back(simTime);

      // Parallel OMP
      Timer parallelOMPTimer;

      simTime = parallelOMPTimer.elapsed();
      parallelOMPTimes.push_back(simTime);

      // Parallel CUDA
      Timer parallelCUDATimer;

      simTime = parallelCUDATimer.elapsed();
      parallelCUDATimes.push_back(simTime);

    }
    displayResults(tests, sequentialTimes, parallelOMPTimes, parallelCUDATimes);
  }
  return 0;
}