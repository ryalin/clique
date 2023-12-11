#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <unistd.h>
#include <iomanip>
#include <getopt.h>
#include <sstream>

#include "../test.h"
#include "../sequential/sequentialRecursive.h"
#include "omp.h"
#include "../timing.h"
#include "../util.h"

// Generates graphs then runs benchmarks on sequential and parallel versions
int main(int argc, char *argv[]) {

  // Input parameters:
  // c: trigger correctness check
  // t: target clique size

  if (argc == 1) {
    std::cout << "Not enough arguments" << std::endl;
    displayUsage();
    return 1;
  }

  const char *short_options = "cbpt:h";
  const struct option long_options[] = {
    {"help", no_argument, nullptr, 'h'},
    {"tests", required_argument, nullptr, 't'},
    {"correctness", no_argument, nullptr, 'c'},
    {"benchmark", no_argument, nullptr, 'b'},
    {"parallel", no_argument, nullptr, 'p'},
    {nullptr, 0, nullptr, 0}
  };

  int option;
  std::string testName;
  bool checkCorrectness = false;
  bool benchmark = false;
  bool parallel = false;

  while ((option = getopt_long(argc, argv, short_options, long_options, nullptr)) != -1) {
    switch (option) {
        case 'c':
          checkCorrectness = true;
          break;
        case 'b':
          benchmark = true;
          break;
        case 'p':
          parallel = true;
          break;
        case 'h':
          displayUsage();
          return 0;
        case '?':
          displayUsage();
          return 1;
        case 't':
          testName = optarg;
          break;
        default:
          std::cerr << "Unknown option: " << static_cast<char>(optopt) << std::endl;
          displayUsage();
          return 1;
    }
  }

  // int option;
  // std::string testName;
  // bool checkCorrectness = false;
  // bool benchmark = false;
  // while ((option = getopt(argc, argv, "cbt:h")) != -1) {
  //   switch (option) {
  //     case 'c':
  //       checkCorrectness = true;
  //       break;
  //     case 'b':
  //       benchmark = true;
  //       break;
  //     case 't':
  //       // testName = optarg;
  //       std::cout << optarg << std::endl;
  //       break;
  //     case 'h':
  //       displayUsage();
  //       return 1;
  //     case '?':
  //       displayUsage();
  //       return 1;
  //     default:
  //       std::cerr << "Unknown option: " << static_cast<char>(optopt) << std::endl;
  //       displayUsage();
  //       return 1;
  //   }
  // }

  if (checkCorrectness) { // Correctness toggle

    std::vector<Graph> tests;
    tests.push_back({5, "one_clique5", readGraphFromTxt("../tests/one_clique5.txt")});
    tests.push_back({15, "one_clique15", readGraphFromTxt("../tests/one_clique15.txt")});
    tests.push_back({30, "one_clique30", readGraphFromTxt("../tests/one_clique30.txt")});
    tests.push_back({45, "one_clique45", readGraphFromTxt("../tests/one_clique45.txt")});

    std::cout << "=============== Correctness Test (OMP) ================" << std::endl;

    for (auto graph: tests) {
      std::cout << "Running test " << graph.name << std::endl;

      bool res1 = parallelClique(graph.nodes, graph.targetSize);
      bool res2 = sequentialRecursive(graph.nodes, graph.targetSize);
      if (res1 != res2) {
        std::cerr << "Error: sequential and parallel returned different results" << std::endl;
        return 0;
      }
      if (res1 == 0) {
        std::cerr << "Error: output should be true" << std::endl;
        return 0;
      }
      // Check if non-existent nodes 
      res1 = parallelClique(graph.nodes, graph.targetSize + 2);
      res2 = sequentialRecursive(graph.nodes, graph.targetSize + 2);
      if (res1 != res2) {
        std::cerr << "Error: sequential and parallel returned different results" << std::endl;
        return 0;
      }
      if (res1 == 1) {
        std::cerr << "Error: output should be false" << std::endl;
        return 0;
      }
    }

    Graph multi;
    multi.nodes = readGraphFromTxt("../tests/multi_clique80.txt");
    for (int i = 0; i < 5; i++) {
      std::cout << "Running test multi_clique" << i * 20 << std::endl;
      bool res1 = parallelClique(multi.nodes, i * 20);
      bool res2 = sequentialRecursive(multi.nodes, i * 20);
      if (res1 != res2) {
        std::cerr << "Error: sequential and parallel returned different results" << std::endl;
        return 0;
      }
      if (res1 == 0) {
        std::cerr << "Error: output should be true" << std::endl;
        return 0;
      }
    }
    std::cout << "All tests passed!" << std::endl;

  // TWEAK THE BENCHMARK TESTS TO NOT RUN FOREVER
  } else if (benchmark) { // Benchmarking

    std::cout << "=============== Benchmark Test (OMP) ================" << std::endl;
    std::vector<Graph> tests;

    tests.push_back({-1, "random_graph_0.2_size20", generateRandom(20, 0.2)});
    tests.push_back({-1, "random_graph_0.4_size20", generateRandom(20, 0.4)});
    tests.push_back({-1, "random_graph_0.6_size20", generateRandom(20, 0.6)});
    tests.push_back({-1, "random_graph_0.8_size20", generateRandom(20, 0.8)});

    tests.push_back({-1, "random_graph_0.2_size30", generateRandom(30, 0.2)});
    tests.push_back({-1, "random_graph_0.4_size30", generateRandom(30, 0.4)});
    tests.push_back({-1, "random_graph_0.6_size30", generateRandom(30, 0.6)});
    tests.push_back({-1, "random_graph_0.8_size30", generateRandom(30, 0.8)});

    std::vector<double> sequentialTimes;
    std::vector<double> parallelOMPTimes;

    for (auto graph: tests) {
      std::cout << "Running test " << graph.name << std::endl; 

      // // Parallel OMP
      std::cout << "parallel" << std::endl;
      Timer parallelOMPTimer;
      parallelClique(graph.nodes, 8);
      double simTime = parallelOMPTimer.elapsed();
      parallelOMPTimes.push_back(simTime);

      // Sequential
      std::cout << "sequential" << std::endl;
      Timer sequentialTimer;
      bool res = sequentialRecursive(graph.nodes, 8);
      simTime = sequentialTimer.elapsed();
      sequentialTimes.push_back(simTime);
      std::cout << res << std::endl;
    }
    displayResults(tests, sequentialTimes, parallelOMPTimes);
  }

  else { // Running singular test

    // Parse input parameters
    std::vector<int> numbers;
    int target;
    for (int i = optind; i < argc; ++i) {
      int num;
      std::istringstream iss(argv[i]);
      if (iss >> num) {
        if (i == argc - 1) {
          target = num;
        } else {
          numbers.push_back(num);
        }
      }
    }

    if (testName == "one") {
      if (numbers.size() != 1) {
        std::cout << "Argument mismatch for test one" << std::endl;
        return 0;
      }
      Graph oneClique;
      oneClique.nodes = generateOneClique(numbers[0]);
      std::cout << "Generated graph with " << numbers[0] << 
      " clique size and searching for target " << target << 
      " with parallel: " << parallel << std::endl;
      bool res;
      if (parallel) {
        res = parallelClique(oneClique.nodes, target);
      } else {
        res = sequentialRecursive(oneClique.nodes, target);
      }
      std::cout << "Result: " << res << std::endl;

    } else if (testName == "random") {
      if (numbers.size() != 1) {
        std::cout << "Argument mismatch for test random" << std::endl;
        return 0;
      }
      Graph random;
      random.nodes = generateRandom(numbers[0], 0.5);
      std::cout << "Generated graph with " << numbers[0] << 
      " size and 0.5 density and searching for target " << target << 
      " with parallel: " << parallel << std::endl;
      bool res;
      if (parallel) {
        res = parallelClique(random.nodes, target);
      } else {
        res = sequentialRecursive(random.nodes, target);
      }
      std::cout << "Result: " << res << std::endl;

    } else if (testName == "multi") {
      if (numbers.size() <= 0) {
        std::cout << "Argument mismatch for test multi" << std::endl;
        return 0;
      }
      Graph multiClique;
      multiClique.nodes = multiCliqueGraph(numbers);
      std::cout << "Generated graph with " << numbers[0] << 
      " size and searching for target " << target << 
      " with parallel: " << parallel << std::endl;
      bool res;
      if (parallel) {
        res = parallelClique(multiClique.nodes, target);
      } else {
        res = sequentialRecursive(multiClique.nodes, target);
      }
      std::cout << "Result: " << res << std::endl;

    } else {
      std::cout << "Not valid test name" << std::endl;
      return 0;
    }
  }
  return 0;
}