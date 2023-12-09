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
  tests.push_back({"one clique 100", readGraphFromTxt("tests/oneClique100.txt")});
  tests.push_back({"one clique 200", readGraphFromTxt("tests/oneClique200.txt")});

  // Multiple cliques with max size of n
  tests.push_back({"multiple cliques 50", readGraphFromTxt("tests/multiClique50.txt")});
  tests.push_back({"multiple cliques 100", readGraphFromTxt("tests/multiClique100.txt")});
  tests.push_back({"multiple cliques 200", readGraphFromTxt("tests/multiClique200.txt")});

  // Random graph of n size
  tests.push_back({"random graph 50", readGraphFromTxt("tests/randomGraph50.txt")});
  tests.push_back({"random graph 100", readGraphFromTxt("tests/randomGraph100.txt")});
  tests.push_back({"random graph 200", readGraphFromTxt("tests/randomGraph200.txt")});
  std::cout << "done\n" << std::endl;

  if (checkCorrectness) { // Correctness toggle

    Graph test;
    // test.nodes = multiCliqueGraph({2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3});
    test.nodes = multiCliqueGraph({6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7});
    printGraph(test);

    std::cout << "OpenMP... " << std::flush; 
    Timer parallelOMPTimer;
    bool returnn = parallelClique(test.nodes, 7);
    double simTime = parallelOMPTimer.elapsed();
    std::cout << simTime << " " << returnn << std::endl;

    // Sequential
    std::cout << "Sequential... " << std::flush; 
    Timer sequentialTimer;
    bool retVal = sequentialRecursive(test.nodes, 7);
    double sim2Time = sequentialTimer.elapsed();
    std::cout << sim2Time << " " << retVal << std::endl;
  
    std::cout << "Speedup: " << sim2Time / simTime << std::endl;
    // std::cout << "=============== Correctness Test ================" << std::endl;
    // for (auto graph: tests) {
    //   std::cout << "Running test " << graph.name << std::endl; 
    //   bool nonRecurse = sequentialClique(graph.nodes, t);
    //   bool recurse = sequentialRecursive(graph.nodes, t);
    //   if (nonRecurse != recurse) {
    //     std::cout << "Correctness Check Failed" << std::endl;
    //   }
    // }
    // std::cout << "Test passed" << std::endl;

  } else { // Benchmarking
    std::cout << "=============== Benchmark Test ================" << std::endl;
    std::vector<double> sequentialTimes;
    std::vector<double> parallelOMPTimes;
    std::vector<double> parallelCUDATimes;

    for (auto graph: tests) {
      std::cout << "\nRunning test " << graph.name << ": " << std::flush; 

      // Parallel OMP
      std::cout << "OpenMP... " << std::flush; 
      Timer parallelOMPTimer;
      bool returnn = parallelClique(graph.nodes, t);
      std::cout << returnn << std::flush; 
      double simTime = parallelOMPTimer.elapsed();
      parallelOMPTimes.push_back(simTime);

      // Sequential
      std::cout << "Sequential... " << std::flush; 
      Timer sequentialTimer;
      bool retVal = sequentialRecursive(graph.nodes, t);
      std::cout << retVal << std::flush; 
      simTime = sequentialTimer.elapsed();
      sequentialTimes.push_back(simTime);

      // Parallel CUDA
      Timer parallelCUDATimer;

      simTime = parallelCUDATimer.elapsed();
      parallelCUDATimes.push_back(simTime);

    }
    displayResults(tests, sequentialTimes, parallelOMPTimes, parallelCUDATimes);
  }
  return 0;
}