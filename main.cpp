#include <iostream>
#include <cstdlib>
#include <vector>

#include "test.h"
#include "sequential.h"
#include "openMP.h"

int main(int argc, const char **argv) {
  // Read input parameters:
  // - graph size, how many cores, etc.

  std::map< int,std::vector<int> > graph = generateGraph(3, 5);
  for (int i = 1; i < graph.size() + 1; i++) {
    std::cout << "NODE " << i  << ": "<< std::endl;
    for (int j = 0; j < graph[i].size(); j++) {
      std::cout << " " << graph[i][j] << " " << std::endl;
    }
  }
  std::cout << "poop" << std::endl;
  bool isClique = sequentialClique(graph, 3);
  std::cout << "result: " << isClique << std::endl;
  // Call graph generation
  // Call sequential algorithm (return timing)
  // Call parallel algorithm (return timin)

  // Display output
  return 0;
}