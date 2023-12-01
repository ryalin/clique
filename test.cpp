#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <set>

// Graph-generating test file


// Given cliqueSize and graphSize, generate graph with singular clique
// of clique size, with total number of nodes being graphSize
std::map<int,std::set<int>> generateOneClique(int cliqueSize, int graphSize) {
  std::map<int,std::set<int>> graph;
  // Create a clique of cliqueSize
  for (int node = 1; node < cliqueSize + 1; node++) {
    for (int neighbor = 1; neighbor < cliqueSize + 1; neighbor++) {
      if (neighbor == node) {
        continue;
      }
      graph[node].insert(neighbor);
    }
  }
  // Connects rest of nodes to clique
  for (int node = cliqueSize + 1; node < graphSize + 1; node++) {
    // Assign other nodes to one of the nodes in the clique
    int corrCliqueNode = (node - cliqueSize) % cliqueSize;
    graph[node].insert(corrCliqueNode);
    graph[corrCliqueNode].insert(node);
  }
  return graph;
}


// Given different cliqueSizes, generates graph with multiple cliques
// connected together, with total number of nodes being graphSize
std::map<int,std::set<int>> multiCliqueGraph(std::vector<int> cliqueSizes, int graphSize) {
  std::map<int,std::set<int>> graph;
  int usedNodes = 1;
  for (auto cliqueSize: cliqueSizes) {
    for (int node = usedNodes; node < cliqueSize + 1; node++) {
      for (int neighbor = usedNodes; neighbor < cliqueSize + 1; neighbor++) {
        if (neighbor == node) {
          continue;
        }
        graph[node].insert(neighbor);
      }
      usedNodes++;
    }
  }
  // Find a single clique in each clique
  std::vector<int> cliqueRep;
  int lastRep = 1;
  for (int i = 0; i < cliqueSizes.size(); i++) {
    cliqueRep.push_back(lastRep);
    lastRep += cliqueSizes[i];
  }
  // Join cliques together with one node from each clique
  for (int i = 1; i < cliqueRep.size(); i++) {
    int prev = cliqueRep[i - 1];
    int curr = cliqueRep[i];
    graph[prev].insert(curr);
    graph[curr].insert(prev);
  }
  return graph;
}


// Generates a random graph with graphSize nodes
std::map<int,std::set<int>> generateRandomGraph(int graphSize) {
  std::map<int,std::set<int>> graph;
  // Generate random device
  std::random_device rd;
  std::mt19937 gen(rd());

  for (int node = 1; node < graphSize + 1; node++) {

    // Get random neighbor list size
    std::uniform_int_distribution<std::size_t> sizeDistribution(1, graphSize);
    std::size_t randomSetSize = sizeDistribution(gen);
    std::uniform_int_distribution<int> numberDistribution(1, graphSize);

    while (graph[node].size() < randomSetSize) {
      // Add random node as neighbor
      int randomNumber = numberDistribution(gen);
      graph[node].insert(randomNumber);
    }
  }
  return graph;
}



