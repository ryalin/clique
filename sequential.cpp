#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <set>


// Helper function to check if a number exists in a vector
bool inVector(std::set<int> neighbors, int targetValue) {
    bool found = false;
    for (int element : neighbors) {
        if (element == targetValue) {
            found = true;
            break;
        }
    }
    return found;
}

// Checks if a given vector is a full clique
bool isClique(std::map<int, std::set<int>> graph, std::set<int> innerVector) {
  for (int element : innerVector){
    std::set<int> curNeighbors = graph[element];
    for (int element2 : innerVector) {
      if (element == element2) continue;
      std::set<int> secondNei = graph[element2];
      if (!inVector(secondNei, element)) return false;
    }
  }
  return true;
}

// Function to generate all combinations of a given size
void generateCombinations(const std::set<int>& input, int size, std::set<int>& current, std::vector<std::set<int>>& result) {
    // If the current combination has reached the desired size, add it to the result
    if (current.size() == size) {
        result.push_back(current);
        return;
    }

    // Recursively generate combinations
    for (const auto& elm : input) {
        current.insert(elm);
        generateCombinations(input, size, current, result);
        current.erase(elm);
    }
}

// Function to get all combinations of a given size from a vector
std::vector<std::set<int>> getAllCombinations(const std::set<int>& input, int size) {
    std::vector<std::set<int>> result;
    std::set<int> currentCombination;
    generateCombinations(input, size, currentCombination, result);
    return result;
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






