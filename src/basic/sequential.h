#include <map>
#include <set>

// Generates all graphs of targetCount size then checks if they are cliques
bool sequentialClique(std::map<int, std::set<int>> graph, int targetCount);
