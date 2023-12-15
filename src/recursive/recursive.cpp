#include <map>
#include <vector>
#include <set>
#include <iostream>

//Checks if the node we want to add is connected to all nodes in curr
bool connected(std::map<int,std::set<int>> graph, std::set<int> curr, int newAdd) {
  std::set<int> newAddNeighbors = graph[newAdd];
  bool ret = true;
  for (int i = 0; i < curr.size(); ++i) {
    auto it = curr.begin();
    std::advance(it, i);
    int neighbor = *it;
    ret = ret && !(newAddNeighbors.find(neighbor) == newAddNeighbors.end());
  }
  return ret;
}

// Recursive funciton to check for cliques
bool recursiveHelper(std::map<int,std::set<int>> graph, int targetCount, 
                     std::set<int> currClique, std::set<int> allNeighbors) {
  if (currClique.size() >= targetCount) return true;
  bool found = false;
  for (std::set<int>::iterator i = allNeighbors.begin(); i != allNeighbors.end(); i++) {
    int neighbor = *i;

    // Make sure we don't check a node that is already in the set 
    if ((currClique.find(neighbor) == currClique.end()) && connected(graph, currClique, neighbor)) {
      std::set<int> newCurrClique = currClique;
      newCurrClique.insert(neighbor);
      bool res = recursiveHelper(graph, targetCount, newCurrClique, allNeighbors);
      if (res) return true; 
    }
  }
  return found;
}

// Recursive clique size-finding wrapper
bool sequentialRecursive(std::map<int,std::set<int>> graph, int targetCount) {
  bool found = false;
  // Loop through all the keys
  for (const auto& entry: graph) {
    int node = entry.first;
    std::set<int> neighbors = graph[node];
    std::set<int> starter = {node};
    bool res = recursiveHelper(graph, targetCount, starter, neighbors);
    if (res)  {
      return true;
    }
  }
  return found;
}
