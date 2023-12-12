#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <iomanip>

// Graph struct: has size, name, and node datastructure
struct Graph {
  int targetSize;
  std::string name;
  std::map<int,std::set<int>> nodes;
};

// Given a txt graph file, returns a map of nodes to their neighbors
std::map<int,std::set<int>> readGraphFromTxt(const std::string& filename) {
  std::map<int,std::set<int>> graph;

  std::ifstream file(filename);
  if (!file.is_open()) {
      std::cerr << "Error opening file: " << filename << std::endl;
      return graph;
  }
  std::string line;
  while (std::getline(file, line)) {
      std::istringstream iss(line);
      int node;
      iss >> node;

      int neighbor;
      std::set<int> neighbors;
      while (iss >> neighbor) {
          neighbors.insert(neighbor);
      }
      graph[node] = neighbors;
  }
  file.close();
  return graph;
}


// Prints contents of a graph
void printGraph(Graph graph){
  std::cout << "\n--- PRINT GRAPH " << graph.name << "---" << std::endl;
  for (auto node: graph.nodes) {
    std::cout << "node: " << node.first << std::endl;
    for (int neighbor: graph.nodes[node.first]) {
      std::cout << neighbor << std::endl;
    }
  }
  std::cout << "--------- END ----------" << std::endl;
}


// Displays final speedups
void displayResults(std::vector<Graph> tests, 
                    std::vector<double> sequentialTimes, 
                    std::vector<double> parallelTimes) {

  std::cout << std::left << std::setw(30) 
  << "\nTest Name" << std::setw(15) << "Seq Time" << std::setw(15) 
  << "Parallel Time" << std::setw(15) << "Parallel Speedup" << std::endl;

  std::cout << "---------------------------------------------------------"
  "----------------------------------------------" << std::endl;

  for (int i = 0; i < tests.size(); i++) {
    double seqTime = sequentialTimes[i];
    double parTime = parallelTimes[i];
    double speedup = seqTime / parTime;

    std::cout << std::left << std::setw(30) 
    << tests[i].name << std::setw(15) << seqTime << std::setw(15) 
    << parTime << std::setw(15) << speedup << std::setw(15) << std::endl;
  }
}


// Displays program usage options
void displayUsage() {
  std::cerr << "\nUsage: ./clique" << " -c -b -p -t <test parameters> -h" << std::endl;
  std::cerr << "Options:" << std::endl;
  std::cerr << "  -c                         Toggle correctness check" << std::endl;
  std::cerr << "  -b                         Toggle benchmark check" << std::endl;
  std::cerr << "  -t <graphParams> <target>  Single test run with paramters" << std::endl;
  std::cerr << "  -p                         Toggle parallel test run" << std::endl;
  std::cerr << "  -h                         Display this help message\n" << std::endl;
}

