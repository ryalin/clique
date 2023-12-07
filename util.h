#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>
#include <unistd.h>
#include <iomanip>
#include <map>

struct Graph {
  std::string name;
  std::map<int,std::set<int>> nodes;
};

std::map<int,std::set<int>> readGraphFromTxt(const std::string& filename);

void displayResults(std::vector<Graph> tests, 
                    std::vector<double> sequentialTimes, 
                    std::vector<double> parallelOMPTimes, 
                    std::vector<double> parallelCUDATimes);

void displayUsage();

void printGraph(Graph graph);