#include <algorithm>
#include <vector>
#include <set>
#include <map>

// Header file for sequential (recursive) Bron-Kerbosch algorithm

// Implementation of bron-kerbosch algorithm
bool bronKerbosch(const std::map<int, std::set<int>>& graph, int k);