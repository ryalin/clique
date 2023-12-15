#include <map>
#include <set>

// Recursively finds cliques of targetCount
bool sequentialRecursive(std::map<int,std::set<int>> graph, int targetCount, bool IMM);
