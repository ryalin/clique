#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <mpi.h>
#include "../timing.h"

//Checks if the node we want to add is connected to all nodes in curr
bool connectedP(std::map<int,std::set<int>> graph, std::set<int> curr, int newAdd) {
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

// Recursive funciton to check for cliques
bool parallelHelper(std::map<int,std::set<int>> graph, int targetCount, 
                    std::set<int> currClique, std::set<int> allNeighbors) {

  if (currClique.size() >= targetCount) return true;
  for (std::set<int>::iterator i = allNeighbors.begin(); i != allNeighbors.end(); i++) {
    int neighbor = *i;

    // Make sure we don't check a node that is already in the set 
    if ((currClique.find(neighbor) == currClique.end()) && connectedP(graph, currClique, neighbor)) {
      std::set<int> newCurrClique = currClique;
      newCurrClique.insert(neighbor);
      if (parallelHelper(graph, targetCount, newCurrClique, allNeighbors)) return true;
    }
  }
  return false;
}

// Recursive clique size-finding wrapper
bool parallelClique(std::map<int,std::set<int>> graph, int targetCount, int argc, char* argv[], double* time) {
  
  MPI_Init(&argc, &argv);

  int pid, nproc;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  Timer timer;
  bool ret = false;
  MPI_Barrier(MPI_COMM_WORLD);
  if (pid == 0) {

    std::cout << "GOT HERE" << std::endl;
    
    // Loop through all the keys
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
    std::cout << "GOT HERE 2 " << pid << std::endl;
    double simTime = timer.elapsed();
    std::cout << "GOT HERE" << std::endl;
    if (time != NULL) {
      *time = simTime;
    }
    std::cout << "GOT HERE 3" << std::endl;
    MPI_Finalize();
  } else {
    std::cout << "GOT HERE 4 " << pid << std::endl;
  }
  std::cout << "GOT HERE 5 " << pid << std::endl;
  return ret;
}
