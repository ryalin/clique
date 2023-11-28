#include <iostream>
#include <cstdlib>
#include <vector>
#include <set>

#include "test.h"
#include "sequential.h"
#include "openMP.h"

int main(int argc, const char **argv) {
  // Read input parameters:
  // - graph size, how many cores, etc.

  std::map< int,std::set<int>> graph = generateOneClique(50, 56);
  for (int i = 1; i < graph.size() + 1; i++) {
    std::cout << "NODE " << i  << ": "<< std::endl;
    for (auto elem: graph[i]) {
      std::cout << " " << elem << " " << std::endl;
    }
  }
  // std::cout << "poop" << std::endl;
  // bool isClique = sequentialClique(graph, 25);
  // std::cout << "result: " << isClique << std::endl;
  // Call graph generation
  // Call sequential algorithm (return timing)
  // Call parallel algorithm (return timin)

  // Display output
  return 0;
}