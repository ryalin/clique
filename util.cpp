#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <string>
#include <map>
#include <iomanip>

struct Graph {
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


// Graph-generating test file
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
                    std::vector<double> parallelOMPTimes,
                    std::vector<double> parallelCUDATimes) {

    std::cout << std::left << std::setw(30) 
    << "\nTest Name" << std::setw(15) << "Seq Time" << std::setw(15) 
    << "OMP Time" << std::setw(15) << "OMP Speedup" << std::setw(15) 
    << "CUDA Time" << std::setw(15) << "CUDA Speedup" << std::endl;

    std::cout << "---------------------------------------------------------"
    "----------------------------------------------" << std::endl;

    for (int i = 0; i < tests.size(); i++) {
      double seqTime = sequentialTimes[i];
      double ompTime = parallelOMPTimes[i];
      double cudaTime = parallelCUDATimes[i];
      double ompSpeedup = seqTime / ompTime;
      double cudaSpeedup = seqTime / cudaTime;

      std::cout << std::left << std::setw(30) 
      << tests[i].name << std::setw(15) << seqTime << std::setw(15) 
      << ompTime << std::setw(15) << ompSpeedup << std::setw(15) 
      << cudaTime << std::setw(15) << cudaSpeedup << "\n" << std::endl;
    }
}


// Displays program usage options
void displayUsage() {
    std::cerr << "\nUsage: ./clique" << " -c -t <targetCliqueSize> -h" << std::endl;
    std::cerr << "Options:" << std::endl;
    std::cerr << "  -c, --optionC           Toggle correctness check" << std::endl;
    std::cerr << "  -t, --optionG <value>   Clique size to search for" << std::endl;
    std::cerr << "  -h, --help              Display this help message\n" << std::endl;
}

