#include <iostream>
#include <vector>
#include <map>
#include <set>

// Given cliqueSize and graphSize, generate graph with singular clique
// of clique size, with total number of nodes being graphSize
std::map<int,std::set<int>> generateOneClique(int cliqueSize, int graphSize);

// Given different cliqueSizes, generates graph with multiple cliques
// connected together, with total number of nodes being graphSize
std::map<int,std::set<int>> multiCliqueGraph(std::vector<int> cliqueSizes, int graphSize);

// Generates a random graph with graphSize nodes
std::map<int,std::set<int>> generateRandomGraph(int graphSize);