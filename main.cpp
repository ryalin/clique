#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <unistd.h>
#include <iomanip>

#include "test.h"
#include "sequential.h"
#include "openMP.h"
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
    std::cout << std::setw(15) << "Test" << std::setw(10) << "Seq Time" <<  
    std::setw(10) << "OMP Time" << std::setw(10) << "OMP Speedup" << std::setw(10) 
    << "CUDA Time" << std::setw(10) << "CUDA Speedup" << std::endl;

    for (int i = 0; i < tests.size(); i++) {
      double seqTime = sequentialTimes[i];
      double ompTime = parallelTimes[i];
      double ompSpeedup = sequentialTimes[i] / parallelTimes[i];

      std::cout << std::setw(15) << tests[i].second << std::setw(10) << seqTime <<  
      std::setw(10) << ompTime << std::setw(10) << ompSpeedup << std::setw(10) 
      << 0 << std::setw(10) << 0 << std::endl;
    }
}


// Generates graphs then runs benchmarks on sequential and parallel versions
int main(int argc, char *argv[]) {
  std::cout << "HELLO" << std::endl;

  // Input parameters:
  // c: trigger correctness
  // t: target clique size

  // Default sizes for graph generation
  int cliqueSize = 2;
  std::vector<int> cliqueSizes = {2, 2, 2, 2, 2};
  int graphSize = 5;

  int option, t;
  bool checkCorrectness = false;
  while ((option = getopt(argc, argv, "ct:h")) != -1) {
    switch (option) {
      case 'c':
        checkCorrectness = true;
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
  std::pair<std::map<int,std::set<int>>,std::string> test1(generateOneClique(cliqueSize, graphSize), "one clique");
  std::pair<std::map<int,std::set<int>>,std::string> test2(multiCliqueGraph(cliqueSizes, graphSize), "multiple cliques");
  std::pair<std::map<int,std::set<int>>,std::string> test3(generateRandomGraph(graphSize), "random graph");
  std::vector<std::pair<std::map<int,std::set<int>>,std::string>> tests = {test1, test2, test3};
  std::cout << "done" << std::endl;

  if (checkCorrectness) {
    // Call openmp version with each graph
    // Call sequential version with each graph
    // Check if outputs are the same

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
      simTime = parallelTimer.elapsed();
      parallelTimes.push_back(simTime);
    }
    displayResults(tests, sequentialTimes, parallelTimes);
  }
  return 0;
}