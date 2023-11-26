#include <iostream>
#include <vector>
#include <map>

// File used for generating graphs

// Given a clique size and graph size, generates graph containing clique
std::map<int,std::vector<int>> generateGraph(int cliqueSize, int graphSize) {

  std::map<int,std::vector<int>> graphMap;
  // Create a clique of cliqueSize
  for (int node = 1; node < cliqueSize + 1; node++) {
    for (int neighbor = 1; neighbor < cliqueSize + 1; neighbor++) {
      if (neighbor == node) {
        continue;
      }
      graphMap[node].push_back(neighbor);
    }
  }
  
  // Connects rest of nodes to clique
  for (int node = cliqueSize + 1; node < graphSize + 1; node++) {
    // Assign other nodes to one of the nodes in the clique
    int corrCliqueNode = (node - cliqueSize) % cliqueSize;
    graphMap[node].push_back(corrCliqueNode);
    graphMap[corrCliqueNode].push_back(node);
  }
  return graphMap;
}

