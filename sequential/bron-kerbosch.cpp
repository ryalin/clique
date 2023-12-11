#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>

// bron-kerbosch recursive helper
bool bronKerboschRecurse(std::set<int>& r, std::set<int>& p, std::set<int>& x,
                            const std::map<int, std::set<int>>& graph, int k) {
    if (r.size() == static_cast<size_t>(k)) {
        return true;
    }
    if (p.empty() && x.empty()) {
        return false;
    }
    int pivot = *p.begin();
    for (int vertex : p) {
        if (graph.find(pivot) != graph.end() && 
            graph.at(pivot).find(vertex) == graph.at(pivot).end()) {
            std::set<int> new_r = r;
            new_r.insert(vertex);
            std::set<int> new_p;
            try {
                std::set_intersection(p.begin(), p.end(), 
                graph.at(vertex).begin(), graph.at(vertex).end(), 
                std::inserter(new_p, new_p.begin()));
            } catch (const std::out_of_range& e) {
    
            }
            std::set<int> new_x;
            try {
                std::set_intersection(x.begin(), x.end(), 
                graph.at(vertex).begin(), graph.at(vertex).end(), 
                std::inserter(new_x, new_x.begin()));
            } catch (const std::out_of_range& e) {
  
            }
            if (bronKerboschRecurse(new_r, new_p, new_x, graph, k)) {
                return true;
            }
            p.erase(vertex);
            x.insert(vertex);
        }
    }
    return false;
}

// Implementation of bron-kerbosch algorithm
bool bronKerbosch(const std::map<int, std::set<int>>& graph, int k) {
    std::set<int> r;
    std::set<int> p;
    std::set<int> x;

    for (const auto& entry : graph) {
        p.insert(entry.first);
    }
    return bronKerboschRecurse(r, p, x, graph, k);
}
