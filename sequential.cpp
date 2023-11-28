#include <iostream>
#include <map>
#include <vector>
#include <algorithm>


// Helper function to check if a number exists in a vector
bool inVector(std::vector<int> neighbors, int targetValue) {
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
bool isClique(std::map<int, std::vector<int>> graph, std::vector<int> innerVector) {
  for (int element : innerVector){
    std::vector<int> curNeighbors = graph[element];
    for (int element2 : innerVector) {
      if (element == element2) continue;
      std::vector<int> secondNei = graph[element2];
      if (!inVector(secondNei, element)) return false;
    }
  }
  return true;
}

// Function to generate all combinations of a given size
void generateCombinations(const std::vector<int>& input, int size, std::vector<int>& current, int index, std::vector<std::vector<int>>& result) {
    // If the current combination has reached the desired size, add it to the result
    if (current.size() == size) {
        result.push_back(current);
        return;
    }

    // Recursively generate combinations
    for (int i = index; i < input.size(); ++i) {
        current.push_back(input[i]);
        generateCombinations(input, size, current, i + 1, result);
        current.pop_back();
    }
}

// Function to get all combinations of a given size from a vector
std::vector<std::vector<int>> getAllCombinations(const std::vector<int>& input, int size) {
    std::vector<std::vector<int>> result;
    std::vector<int> currentCombination;
    generateCombinations(input, size, currentCombination, 0, result);
    return result;
}

// sequential clique solving algorithm
bool sequentialClique(std::map<int, std::vector<int>> graph, int targetCount) {
  // Loop through all the keys
  for (auto const& entry : graph) {
    if (graph[entry.first].size() >= targetCount - 1) {
      std::vector<int> numbers = graph[entry.first];
      numbers.push_back(entry.first);
      std::vector<std::vector<int>> combs = getAllCombinations(numbers, targetCount);
      for (const auto& innerVector : combs){
        if (isClique(graph, innerVector)) return true;
      }
      numbers.pop_back();
    }
  }
  return false;
}






