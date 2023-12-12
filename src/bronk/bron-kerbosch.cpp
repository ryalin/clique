#include <algorithm>
#include <vector>
#include <set>
#include <map>

// Given 2 sets, finds the intersection and populates into result
void setIntersection(const std::set<int>& set1, const std::set<int>& set2, 
                     std::set<int>& result) {
    result.clear();
    set_intersection(set1.begin(), set1.end(), set2.begin(), 
                     set2.end(), inserter(result, result.begin()));
}


// Bron-Kerbosch algorithm recursive helper
bool bronKerboschRecurse(std::set<int>& R, std::set<int>& P, std::set<int>& X, 
                         const std::map<int, std::set<int>>& graph, int k) {
    if (P.empty() && X.empty()) {
        if (R.size() >= k) {
            return true;
        }
        return false;
    }

    std::set<int> P_copy = P;
    for (int v : P_copy) {
        std::set<int> neighbors = graph.at(v);
        std::set<int> P_intersection;
        std::set<int> X_intersection;

        R.insert(v);
        setIntersection(P, neighbors, P_intersection);
        setIntersection(X, neighbors, X_intersection);

        bool res = bronKerboschRecurse(R, P_intersection, X_intersection, graph, k);
        if (res) {
            return true;
        }
        R.erase(v);
        P.erase(v);
        X.insert(v);
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

