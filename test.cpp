#include <iostream>
#include <vector>
#include <map>
#include <set>

// File used for generating graphs

// Given a clique size and graph size, generates graph containing clique
std::map<int,std::set<int>> generateOneClique(int cliqueSize, int graphSize) {

  std::map<int,std::set<int>> graphMap;
  // Create a clique of cliqueSize
  for (int node = 1; node < cliqueSize + 1; node++) {
    for (int neighbor = 1; neighbor < cliqueSize + 1; neighbor++) {
      if (neighbor == node) {
        continue;
      }
      graphMap[node].insert(neighbor);
    }
  }
  // Connects rest of nodes to clique
  for (int node = cliqueSize + 1; node < graphSize + 1; node++) {
    // Assign other nodes to one of the nodes in the clique
    int corrCliqueNode = (node - cliqueSize) % cliqueSize;
    graphMap[node].insert(corrCliqueNode);
    graphMap[corrCliqueNode].insert(node);
  }
  return graphMap;
}



