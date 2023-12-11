#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>

//Checks if the node we want to add is connected to all nodes in curr
bool neiConnectedP2(std::map<int,std::set<int>> graph, std::set<int> curr, int newAdd) {
  std::set<int> newAddNeighbors = graph[newAdd];
  bool ret = true;
  #pragma omp parallel for
  for (int i = 0; i < curr.size(); ++i) {
    auto it = curr.begin();
    std::advance(it, i);
    int neighbor = *it;
    ret = ret && !(newAddNeighbors.find(neighbor) == newAddNeighbors.end());
  }
  return ret;
}

bool neiConnectedP(std::map<int,std::set<int>> graph, std::set<int> curr, int newAdd) {
  std::set<int> newAddNeighbors = graph[newAdd];
  // #pragma omp parallel for
  for (std::set<int>::iterator i = curr.begin(); i != curr.end(); i++) {
    int neighbor = *i;
    if (newAddNeighbors.find(neighbor) == newAddNeighbors.end()) return false;
  }
  return true;
}

// Recursive funciton to check for cliques
bool parallelHelper(std::map<int,std::set<int>> graph, int targetCount, 
                    std::set<int> currClique, std::set<int> allNeighbors) {

  if (currClique.size() >= targetCount) return true;
  for (std::set<int>::iterator i = allNeighbors.begin(); i != allNeighbors.end(); i++) {
    int neighbor = *i;

    // Make sure we don't check a node that is already in the set 
    if ((currClique.find(neighbor) == currClique.end()) && 
         neiConnectedP2(graph, currClique, neighbor)) {
      std::set<int> newCurrClique = currClique;
      newCurrClique.insert(neighbor);
      if (parallelHelper(graph, targetCount, newCurrClique, allNeighbors)) return true;
    }
  }
  return false;
}

// Recursive clique size-finding wrapper
bool parallelClique(std::map<int,std::set<int>> graph, int targetCount) {

  // Loop through all the keys
  bool ret = false;
  //std::vector<int> keys = getKeys(graph);
  #pragma omp parallel 
  #pragma omp single
  for (int i = 0; i < graph.size(); i++) {
    if (ret) continue;
    int key = i;
    std::set<int> val = graph[key];
    if (val.size() + 1 < targetCount) continue;
    #pragma omp task shared(ret) if (!ret)
    {
    std::set<int> starter = {key};
    std::set<int> neighbors = val;
    bool res = parallelHelper(graph, targetCount, starter, neighbors);
    #pragma omp atomic
    ret = ret | res;
    }
  }
  return ret;
}
