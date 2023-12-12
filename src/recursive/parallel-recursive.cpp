#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>
#include <mpi.h>
#include <getopt.h>

#include "recursive.h"
#include "../test.h"
#include "../timing.h"
#include "../util.h"


// // Function to convert a map to an adjacency matrix
// std::vector<std::vector<int>> mapToAdjacencyMatrix(const std::map<int, std::set<int>>& graph) {
//     // Find the maximum node ID to determine the size of the matrix
//     int maxNodeID = 0;
//     for (const auto& entry : graph) {
//         maxNodeID = std::max(maxNodeID, entry.first);
//         for (int neighbor : entry.second) {
//             maxNodeID = std::max(maxNodeID, neighbor);
//         }
//     }

//     // Initialize the matrix with zeros
//     std::vector<std::vector<int>> adjacencyMatrix(maxNodeID + 1, std::vector<int>(maxNodeID + 1, 0));

//     // Fill in the matrix based on the graph
//     for (const auto& entry : graph) {
//         int node = entry.first;
//         for (int neighbor : entry.second) {
//             adjacencyMatrix[node][neighbor] = 1;
//             adjacencyMatrix[neighbor][node] = 1;  // Assuming an undirected graph
//         }
//     }
//     return adjacencyMatrix;
// }


// // Function to convert an adjacency matrix to a map
// std::map<int, std::set<int>> adjacencyMatrixToMap(const std::vector<std::vector<int>>& adjacencyMatrix) {
//     std::map<int, std::set<int>> graph;

//     // Iterate through the rows and columns of the matrix
//     for (int i = 0; i < adjacencyMatrix.size(); ++i) {
//         for (int j = 0; j < adjacencyMatrix[i].size(); ++j) {
//             // If there is an edge (matrix[i][j] is non-zero), add it to the map
//             if (adjacencyMatrix[i][j] != 0) {
//                 graph[i].insert(j);
//                 graph[j].insert(i);  // Assuming an undirected graph
//             }
//         }
//     }
//     return graph;
// }


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

bool parallelRecursive(std::map<int,std::set<int>> graph, int targetCount) {
  int pid, nproc;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);

  int graphSize = graph.size();
  int chunkSize = (graphSize + nproc - 1) / nproc;
  int lower = pid * chunkSize;
  int upper = std::min((pid + 1) * chunkSize, graphSize);

  bool ret = false;
  #pragma omp parallel 
  #pragma omp single
  // for (int i = chunkSize * pid; i < chunkSize * (pid + 1); i++) {
  for (int i = lower; i < upper; i++) {
    if (ret) continue;
    int key = i;
    std::set<int> val = graph[key];
    if (val.size() + 1 < targetCount) continue;
    #pragma omp task shared(ret) if (!ret)
    {
    std::set<int> starter = {key};
    std::set<int> neighbors = val;
    #pragma omp taskwait
    bool res = parallelHelper(graph, targetCount, starter, neighbors);
    #pragma omp atomic
    ret = ret | res;
    }
  }

  std::vector<int> gatheredBooleans(nproc);
  MPI_Gather(&ret, 1, MPI_INT, gatheredBooleans.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (pid == 0) {
    bool newRet;
    for (int i = 0; i < nproc; i++) {
      newRet = newRet | gatheredBooleans[i];
    }
    std::cout << newRet << std::endl;
    return newRet;
  }
  return ret;
}

// Once sorted out, delete main.cpp, omp.cpp. omp.h
// Recursive clique size-finding wrapper
int main(int argc, char *argv[]) {

  // Run instructions:
  // make run FLAGS="-s <size> -d <density> -t <target> -h -c"

  // Inputs:
  // -t <size>        size of clique to look for
  // -s <size>        size of graph generated
  // -d <density>     density of graph from 0 to 1

  // Optional:
  // -h               displays help message
  // -c               correctness check with test cases

  MPI_Init(&argc, &argv);

  int pid;
  MPI_Comm_rank(MPI_COMM_WORLD, &pid);

  int option;
  int s = 30;
  int t = 10;
  float d = 1;
  bool checkCorrectness = false;
  while ((option = getopt(argc, argv, "cs:d:t:h")) != -1) {
      switch (option) {
      case 's':
          s = atoi(optarg);
          break;
      case 'c':
          checkCorrectness = true;
          break;
      case 'd':
          d = std::stof(optarg);
          break;
      case 't':
          t = atoi(optarg);
          break;
      case 'h':
          displayUsage();
          MPI_Finalize();
          return 0;
      case '?':
          displayUsage();
          MPI_Finalize();
          return 0;
      default:
          std::cerr << "Unknown option: " << static_cast<char>(optopt) << std::endl;
          displayUsage();
          MPI_Finalize();
          return 0;
      }
  }

  if ((checkCorrectness) && (pid == 0)) {
    std::vector<Graph> tests;
    tests.push_back({4, "one_clique5", readGraphFromTxt("../tests/one_clique5.txt")});
    tests.push_back({14, "one_clique15", readGraphFromTxt("../tests/one_clique15.txt")});
    tests.push_back({29, "one_clique30", readGraphFromTxt("../tests/one_clique30.txt")});

    std::cout << "---------------- Correctness Test (OMP) -----------------" << std::endl;

    for (auto graph: tests) {
      std::cout << "Running test " << graph.name << std::endl;

      // Check if clique is in graph
      bool res1 = parallelRecursive(graph.nodes, graph.targetSize);
      bool res2 = sequentialRecursive(graph.nodes, graph.targetSize);
      if (res1 != res2) {
        std::cerr << "Error: sequential and parallel returned different results" << std::endl;
        MPI_Finalize();
        return 0;
      }
      if (res1 == 0) {
        std::cerr << "Error: output should be true" << std::endl;
        MPI_Finalize();
        return 0;
      }
      // Check if target size clique is non-existent
      res1 = parallelRecursive(graph.nodes, graph.targetSize + 2);
      res2 = sequentialRecursive(graph.nodes, graph.targetSize + 2);
      if (res1 != res2) {
        std::cerr << "Error: sequential and parallel returned different results" << std::endl;
        MPI_Finalize();
        return 0;
      }
      if (res1 == 1) {
        std::cerr << "Error: output should be false" << std::endl;
        MPI_Finalize();
        return 0;
      }
    }
    Graph multi;
    multi.nodes = readGraphFromTxt("../tests/multi_clique.txt");
    for (int i = 0; i < 5; i++) {
      std::cout << "Running test multi_clique" << i * 20 << std::endl;
      bool res1 = parallelRecursive(multi.nodes, i * 20);
      bool res2 = sequentialRecursive(multi.nodes, i * 20);
      if (res1 != res2) {
        std::cerr << "Error: sequential and parallel returned different results" << std::endl;
        MPI_Finalize();
        return 0;
      }
      if (res1 == 0) {
        std::cerr << "Error: output should be true" << std::endl;
        MPI_Finalize();
        return 0;
      }
    }
    std::cout << "All tests passed!" << std::endl;
    MPI_Finalize();
    return 0;
  }
  // Generate random graph with command line size and density
  std::map<int, std::set<int>> graph = generateRandom(s, d);

  if (pid == 0) {
    printf("\n------ Test with graph size %d, density %.2f, target %d ------\n", s, d, t);
  }

  Timer parallelTimer;
  bool parRes = parallelRecursive(graph, t);
  double parSimTime = parallelTimer.elapsed();

  Timer seqTimer;
  bool seqRes = sequentialRecursive(graph, t);
  double seqSimTime = seqTimer.elapsed();

  if (pid == 0) {
    std::cout << "SEQ OUT "<<  seqRes << std::endl;
      if (parRes != seqRes) {
          std::cout << "Error: Parallel and Sequential Returned Different Values" << std::endl;
          MPI_Finalize();
          return 0;
      }
      std::cout << std::left << "\n" << std::setw(15) << "Sequential" << std::setw(15) 
      << "Parallel" << std::setw(15) << "Speedup" << std::setw(15) 
      << "Contains Clique?" << std::endl;

      std::cout << "--------------------------------------------------------------" << std::endl;

      std::cout << std::left << std::setw(15) << seqSimTime << std::setw(15) 
      << parSimTime << std::setw(15) << seqSimTime / parSimTime << std::setw(15) 
      << std::setw(15) << parRes << "\n" << std::endl;
  }
  MPI_Finalize();
  return 0;
}
