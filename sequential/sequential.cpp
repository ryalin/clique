#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>



// Checks if a given vector is a full clique
bool isClique(std::map<int, std::set<int>> graph, std::set<int> innerVector) {
  for (int element : innerVector){
    std::set<int> curNeighbors = graph[element];
    for (int element2 : innerVector) {
      if (element == element2) continue;
      std::set<int> secondNei = graph[element2];
      if (secondNei.find(element) == secondNei.end()) return false;
    }
  }
  return true;
}

void generateCombinations(const std::vector<int>& elements, std::size_t setSize, std::size_t index,
                           std::set<int>& currentCombination, std::vector<std::set<int>>& resultSets) {
    if (setSize == 0) {
        resultSets.push_back(currentCombination);
        return;
    }

    for (std::size_t i = index; i < elements.size(); ++i) {
        currentCombination.insert(elements[i]);
        generateCombinations(elements, setSize - 1, i + 1, currentCombination, resultSets);
        currentCombination.erase(elements[i]);
    }
}

std::vector<std::set<int>> generateSetsOfSize(const std::set<int>& inputSet, std::size_t setSize) {
    std::vector<int> inputVector(inputSet.begin(), inputSet.end());
    std::vector<std::set<int>> resultSets;
    std::set<int> currentCombination;

    generateCombinations(inputVector, setSize, 0, currentCombination, resultSets);

    return resultSets;
}

// Function to get all combinations of a given size from a vector
std::vector<std::set<int>> getAllCombinations(const std::set<int> input, int size) {
    std::vector<std::set<int>> resultSets = generateSetsOfSize(input, size);
    //std::cout << "Generated sets of size " << size << ":" << resultSets.size() << std::endl;
    for (const auto& resultSet : resultSets) {
        for (const auto& element : resultSet) {
            //std::cout << element << " ";
        }
        //std::cout << std::endl;
    }
    return resultSets;
}

// sequential clique solving algorithm
bool sequentialClique(std::map<int,std::set<int>> graph, int targetCount) {
  //std::cout << "target size: " << targetCount << std::endl;
  // Loop through all the keys
  for (auto const& entry : graph) {
    if (graph[entry.first].size() >= targetCount - 1) {
      std::set<int> numbers = graph[entry.first];
      numbers.insert(entry.first);
      std::vector<std::set<int>> combs = getAllCombinations(numbers, targetCount);
      for (const auto& innerVector : combs){
        if (isClique(graph, innerVector)) return true;
      }
      numbers.erase(entry.first);
    }
  }
  return false;
}


// /**
//   Given a node in the graph this function returns a set
//    with all of the nieghbors of its neighbors
//  **/
// std::set<int> generateNeighbors(std::map<int,std::set<int>> graph, int node) {
//   std::set<int> allNeighbors = {};
//   for (auto const& neigh : curr) {
//     std::set<int> curNeigh = graph[neigh];
//     std::set_union(allNeighbors.begin(), allNeighbors.end(), curNeigh.begin(), curNeigh.end(), std::inserter(allNeighbors, allNeighbors.begin()));
//   }
// }

//Checks if the node we want to add is connected to all nodes in curr
bool neiConnected(std::map<int,std::set<int>> graph, std::set<int> curr, int newAdd) {
  std::set<int> newAddNeighbors = graph[newAdd];
  for (int neighbor: curr) {
    if (newAddNeighbors.find(neighbor) == newAddNeighbors.end()) return false;
  }
  return true;
}

//recursive funciton to check for cliques
bool recursiveHelper(std::map<int,std::set<int>> graph, int targetCount, std::set<int> curr, std::set<int> allNeighbors) {
  if (curr.size() >= targetCount) return true;
  for (int nei: allNeighbors) {
    // Make sure we don't check a node that is already in the set 
    if ((curr.find(nei) == curr.end()) && neiConnected(graph, curr, nei)) {
      std::set<int> newCurr = curr;
      newCurr.insert(nei);
      if (recursiveHelper(graph, targetCount, newCurr, allNeighbors)) return true;
    }
  }
  return false;
}


// sequential clique solving algorithm
bool sequentialRecursive(std::map<int,std::set<int>> graph, int targetCount) {
  // Loop through all the keys
  for (auto const& entry : graph) {
    int node = entry.first;
    if (graph[node].size() + 1 < targetCount) continue;
    std::set<int> starter = {node};
    std::set<int> neighbors = graph[node];
    //std::cout << "in" << std::endl;
    if (recursiveHelper(graph, targetCount, starter, neighbors)) {
      return true;
    }
    //std::cout << "out" << std::endl;
  }
  return false;
}


//lec 5 starting from slide 20