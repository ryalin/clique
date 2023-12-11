#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <unistd.h>
#include <iomanip>

#include "../test.h"
#include "../sequential/sequential.h"
#include "../sequential/sequentialRecursive.h"
#include "omp.h"
#include "../timing.h"
#include "../util.h"

// Generates graphs then runs benchmarks on sequential and parallel versions
int main(int argc, char *argv[]) {

  // Input parameters:
  // c: trigger correctness with certain clique size
  // t: target clique size

  if (argc == 1) {
    std::cout << "Not enough arguments" << std::endl;
    displayUsage();
    return 1;
  }

  int option, t, c;
  bool checkCorrectness = false;
  while ((option = getopt(argc, argv, "c:t:h")) != -1) {
    switch (option) {
      case 'c':
        checkCorrectness = true;
        c = atoi(optarg);
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

  if (checkCorrectness) { // Correctness toggle

    std::cout << "=============== Correctness Test (OMP) ================" << std::endl;
    Graph test;

    // test.nodes = multiCliqueGraph({7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 7});
    // test.nodes = generateOneClique(200, 2000);
    test.nodes = generateRandomGraph(50);
    // printGraph(test);

    std::cout << "OpenMP... " << std::flush; 
    Timer parallelOMPTimer;
    bool res = parallelClique(test.nodes, c);
    double simTime = parallelOMPTimer.elapsed();
    std::cout << res << std::endl;

    // Sequential
    std::cout << "Sequential... " << std::flush; 
    Timer sequentialTimer;
    res = sequentialRecursive(test.nodes, c);
    double sim2Time = sequentialTimer.elapsed();
    std::cout << res << std::endl;
  
    std::cout << "Speedup: " << sim2Time / simTime << std::endl;

  } else { // Benchmarking
    std::vector<Graph> tests;
    std::cout << "Reading graphs..." << std::flush;
    // One clique graphs with size n, and total size 2n
    tests.push_back({"one clique 50", readGraphFromTxt("../tests/oneClique50.txt")});
    tests.push_back({"one clique 100", readGraphFromTxt("../tests/oneClique100.txt")});
    tests.push_back({"one clique 200", readGraphFromTxt("../tests/oneClique200.txt")});

    // Multiple cliques with max size of n
    tests.push_back({"multiple cliques 50", readGraphFromTxt("../tests/multiClique50.txt")});
    tests.push_back({"multiple cliques 100", readGraphFromTxt("../tests/multiClique100.txt")});
    tests.push_back({"multiple cliques 200", readGraphFromTxt("../tests/multiClique200.txt")});

    // Random graph of n size
    tests.push_back({"random graph 50", readGraphFromTxt("../tests/randomGraph50.txt")});
    tests.push_back({"random graph 100", readGraphFromTxt("../tests/randomGraph100.txt")});
    tests.push_back({"random graph 200", readGraphFromTxt("../tests/randomGraph200.txt")});
    std::cout << "done" << std::endl;

    std::cout << "=============== Benchmark Test (OMP) ================" << std::endl;
    std::vector<double> sequentialTimes;
    std::vector<double> parallelOMPTimes;

    for (auto graph: tests) {
      std::cout << "Running test " << graph.name << " to find size " << t << std::endl; 

      // Parallel OMP
      Timer parallelOMPTimer;
      parallelClique(graph.nodes, t);
      double simTime = parallelOMPTimer.elapsed();
      parallelOMPTimes.push_back(simTime);

      // Sequential
      Timer sequentialTimer;
      sequentialRecursive(graph.nodes, t);
      simTime = sequentialTimer.elapsed();
      sequentialTimes.push_back(simTime);
    }
    displayResults(tests, sequentialTimes, parallelOMPTimes);
  }
  return 0;
}