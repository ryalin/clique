#include <iostream>
#include <vector>
#include <random>
#include <map>
#include <set>

// Given cliqueSize and graphSize, generate graph with singular clique
// of clique size, with total number of nodes being graphSize
std::map<int,std::set<int>> generateOneClique(int cliqueSize) {
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
std::map<int, std::set<int>> generateRandom(int graphSize, double edgeProb) {
    std::map<int, std::set<int>> graph;

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    // Generate edges based on the probability
    for (int i = 0; i < graphSize; ++i) {
        for (int j = i + 1; j < graphSize; ++j) {
            if (dis(gen) < edgeProb) {
                graph[i].insert(j);
                graph[j].insert(i);
            }
        }
    }
    return graph;
}





