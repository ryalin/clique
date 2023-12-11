#include <iostream>
#include <vector>
#include <map>
#include <set>

// Given cliqueSize and graphSize, generate graph with singular clique
std::map<int,std::set<int>> generateOneClique(int cliqueSize);

// Given different cliqueSizes, generates graph with multiple cliques
// connected together, with total number of nodes being graphSize
std::map<int,std::set<int>> multiCliqueGraph(std::vector<int> cliqueSizes);

// Generates a random graph with graphSize nodes
std::map<int, std::set<int>> generateRandom(int graphSize, double edgeProb);