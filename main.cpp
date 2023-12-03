#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <unistd.h>
#include <iomanip>

#include "test.h"
#include "sequential.h"
#include "parallel.h"
#include "timing.h"

// Displays program usage options
void displayUsage() {
    std::cerr << "Usage: ./clique" << " -c -t <targetCliqueSize> -h" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  -c, --optionC           Toggle correctness check" << std::endl;
    std::cerr << "  -t, --optionG <value>   Clique size to search for" << std::endl;
    std::cerr << "  -h, --help              Display this help message" << std::endl;
}


// Displays final speedups
void displayResults(std::vector<std::pair<std::map<int,std::set<int>>,std::string>> tests, 
                    std::vector<double> sequentialTimes, std::vector<double> parallelTimes) {
    std::cout << std::left << std::setw(20) << "Test Name" << std::setw(15) << "Seq Time" <<  
    std::setw(15) << "OMP Time" << std::setw(15) << "OMP Speedup" << std::setw(15) 
    << "CUDA Time" << std::setw(15) << "CUDA Speedup" << std::endl;

    std::cout << "---------------------------------------------------------------------------------------------" << std::endl;

    for (int i = 0; i < tests.size(); i++) {
      double seqTime = sequentialTimes[i];
      double ompTime = parallelTimes[i];
      double ompSpeedup = sequentialTimes[i] / parallelTimes[i];

      std::cout << std::left << std::setw(20) << tests[i].second << std::setw(15) << seqTime <<  
      std::setw(15) << ompTime << std::setw(15) << ompSpeedup << std::setw(15) 
      << 0 << std::setw(15) << 0 << std::endl;
    }
}


// Generates graphs then runs benchmarks on sequential and parallel versions
int main(int argc, char *argv[]) {

  // Input parameters:
  // c: trigger correctness
  // t: target clique size

  // Default sizes for graph generation
  int cliqueSize = 50;
  std::vector<int> cliqueSizes = {20, 20, 20, 20, 20};
  int graphSize = 100;

  int option, t;
  bool checkCorrectness = false;
  while ((option = getopt(argc, argv, "ct:h")) != -1) {
    switch (option) {
      case 'c':
        checkCorrectness = true;
        t = 5;
        break;
      case 't':
        t = atoi(optarg);
        break;
      case 'h':
        displayUsage();
        return 1;
      case ':':
        std::cerr << "Option -" << static_cast<char>(optopt) << " requires an argument." << std::endl;
        displayUsage();
        return 1;
      default:
        std::cerr << "Unknown option: " << static_cast<char>(optopt) << std::endl;
        displayUsage();
        return 1;
    }
  }

  std::cout << "Generating testing graphs...";
  std::pair<std::map<int,std::set<int>>,std::string> test1(generateOneClique(cliqueSize, graphSize), "One Clique");
  std::pair<std::map<int,std::set<int>>,std::string> test2(multiCliqueGraph(cliqueSizes), "Multiple Cliques");
  std::pair<std::map<int,std::set<int>>,std::string> test3(generateRandomGraph(graphSize), "Random Graph");
  std::vector<std::pair<std::map<int,std::set<int>>,std::string>> tests = {test1, test2, test3};
  std::cout << "done" << std::endl;

  if (checkCorrectness) {
    for (int i = 0; i < tests.size(); i++) {
      std::cout << "Running test " << tests[i].second << std::endl; 
      bool nonRecurse = sequentialClique(tests[i].first, t);
      bool recurse = sequentialRecursive(tests[i].first, t);
      if (nonRecurse != recurse) {
        std::cout << "Correctness Check Failed" << std::endl;
      }
    }
    std::cout << "Test passed" << std::endl;

  } else {
    std::vector<double> sequentialTimes;
    std::vector<double> parallelTimes;
    for (int i = 0; i < tests.size(); i++) {
      std::cout << "Running test " << tests[i].second << std::endl; 

      Timer sequentialTimer;
      sequentialClique(tests[i].first, t);
      double simTime = sequentialTimer.elapsed();
      sequentialTimes.push_back(simTime);

      Timer parallelTimer;
      // Call parallel version of thingy
      // sequentialCliqueP(tests[i].first, t);
      // sequentialRecursive(tests[i].first, t);
      parallelClique(tests[i].first, t);


      simTime = parallelTimer.elapsed();
      parallelTimes.push_back(simTime);
    }
    displayResults(tests, sequentialTimes, parallelTimes);
  }
  return 0;
}