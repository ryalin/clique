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
    std::cout << "Generated sets of size " << size << ":" << resultSets.size() << std::endl;
    for (const auto& resultSet : resultSets) {
        for (const auto& element : resultSet) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
    return resultSets;
}

// sequential clique solving algorithm
bool sequentialClique(std::map<int,std::set<int>> graph, int targetCount) {
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
    std::cout << "Loop" << std::endl;
  }
  return false;
}






