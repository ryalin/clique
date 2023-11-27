// Header file to store sequential function defenitions 
#include <iostream>
#include <map>

/** This function solves the clique decision problem given the graph and target
clique size */
bool sequentialClique(std::map<int, std::vector<int>> graph, int targetCount);

bool isClique(std::map<int, std::vector<int>> graph, std::vector<int> innerVector);