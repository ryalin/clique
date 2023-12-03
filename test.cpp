#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <set>

// Graph-generating test file
void printGraph(std::map<int,std::set<int>> graph){
    std::cout << "\n---------PRINT GRAPH ----------" << std::endl;
    for (auto const& entry : graph) {
      std::cout << "node: " << entry.first << std::endl;
      for (int nei: graph[entry.first]) {
        std::cout << nei << std::endl;
      }
    }
    std::cout << "---------END ----------" << std::endl;
}

// Given cliqueSize and graphSize, generate graph with singular clique
// of clique size, with total number of nodes being graphSize
std::map<int,std::set<int>> generateOneClique(int cliqueSize, int graphSize) {
  std::map<int,std::set<int>> graph;
  // Create a clique of cliqueSize
  for (int node = 0; node < cliqueSize; node++) {
    for (int neighbor = 0; neighbor < cliqueSize; neighbor++) {
      if (neighbor == node) {
        continue;
      }
      graph[node].insert(neighbor);
    }
  }
  // Connects rest of nodes to clique
  for (int node = cliqueSize; node < graphSize; node++) {
    // Assign other nodes to one of the nodes in the clique
    int corrCliqueNode = (node - cliqueSize) % cliqueSize;
    graph[node].insert(corrCliqueNode);
    graph[corrCliqueNode].insert(node);
  }
  return graph;
}


// Given different cliqueSizes, generates graph with 
// multiple cliques connected together
std::map<int,std::set<int>> multiCliqueGraph(std::vector<int> cliqueSizes) {
  std::map<int,std::set<int>> graph;
  int usedNodes = 0;
  for (auto cliqueSize: cliqueSizes) {
    for (int node = usedNodes; node < cliqueSize + usedNodes; node++) {
      for (int neighbor = usedNodes; neighbor < cliqueSize + usedNodes; neighbor++) {
        if (neighbor == node) {
          continue;
        }
        graph[node].insert(neighbor);
      }
    }
    usedNodes += cliqueSize;
  }
  // Find a single clique in each clique
  std::vector<int> cliqueRep;
  int lastRep = 0;
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
    std::uniform_int_distribution<std::size_t> sizeDistribution(1, graphSize-1);
    std::size_t randomSetSize = sizeDistribution(gen);
    std::uniform_int_distribution<int> numberDistribution(1, graphSize);

    while (graph[node].size() < randomSetSize) {
      // Add random node as neighbor
      int randomNumber = numberDistribution(gen);
      if (node != randomNumber) {
        graph[node].insert(randomNumber);
        graph[randomNumber].insert(node);
      }
    }
  }
  return graph;
}





