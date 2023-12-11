#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <unistd.h>
#include <iomanip>
#include <map>

// Graph struct: has size, name, and node datastructure
struct Graph {
  int targetSize;
  std::string name;
  std::map<int,std::set<int>> nodes;
};

// Given a txt graph file, returns a map of nodes to their neighbors
std::map<int,std::set<int>> readGraphFromTxt(const std::string& filename);

// Displays final speedups
void displayResults(std::vector<Graph> tests, 
                    std::vector<double> sequentialTimes, 
                    std::vector<double> parallelTimes);


// Displays program usage options
void displayUsage();

// Prints contents of a graph
void printGraph(Graph graph);