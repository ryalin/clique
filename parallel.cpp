#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include "sequential.h"

//Checks if the node we want to add is connected to all nodes in curr
bool neiConnectedP(std::map<int,std::set<int>> graph, std::set<int> curr, int newAdd) {
  std::set<int> newAddNeighbors = graph[newAdd];
  for (std::set<int>::iterator i = curr.begin(); i != curr.end(); ++i) {
    int neighbor = *i;
    if (newAddNeighbors.find(neighbor) == newAddNeighbors.end()) return false;
  }
  return true;
}

//recursive funciton to check for cliques
bool recursiveHelperP(std::map<int,std::set<int>> graph, int targetCount, std::set<int> curr, std::set<int> allNeighbors) {
  if (curr.size() >= targetCount) return true;
  for (std::set<int>::iterator i = allNeighbors.begin(); i != allNeighbors.end(); ++i) {
    int nei = *i;
    // Make sure we don't check a node that is already in the set 
    if ((curr.find(nei) == curr.end()) && neiConnectedP(graph, curr, nei)) {
      std::set<int> newCurr = curr;
      newCurr.insert(nei);
      if (recursiveHelperP(graph, targetCount, newCurr, allNeighbors)) return true;
    }
  }
  return false;
}


// sequential clique solving algorithm
bool parallelClique(std::map<int,std::set<int>> graph, int targetCount) {
  // Loop through all the keys
  bool ret = false;
  std::vector<int> keys = getKeys(graph);
  #pragma omp parallel for
  for (int i = 0; i < graph.size(); i++) {
    int key = keys[i];
    std::set<int> val = graph[key];
    if (val.size() + 1 < targetCount) continue;
    std::set<int> starter = {key};
    std::set<int> neighbors = val;
    //std::cout << "in" << std::endl;
    #pragma omp task
    ret = ret || recursiveHelperP(graph, targetCount, starter, neighbors);
  }
  return ret;
}
