#include <algorithm>
#include <vector>
#include <set>
#include <map>

// Header file for sequential (recursive) Bron-Kerbosch algorithm

// Implementation of bron-kerbosch algorithm
bool bronKerbosch(const std::map<int, std::set<int>>& graph, int k);

void setIntersection(const std::set<int>& set1, const std::set<int>& set2, 
                      std::set<int>& result);