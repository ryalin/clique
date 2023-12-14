#include <algorithm>
#include <vector>
#include <set>
#include <map>

void setIntersection(const std::set<int>& set1, const std::set<int>& set2, 
                      std::set<int>& result) {
    result.clear();
    std::set_intersection(set1.begin(), set1.end(), set2.begin(), 
                          set2.end(), std::inserter(result, result.begin()));
}

bool bronKerboschRecurse(std::set<int>& R, std::set<int>& P, std::set<int>& X, 
                          const std::map<int, std::set<int>>& graph, int k, bool& cliqueFound) {
    if (R.size() >= k) {
        cliqueFound = true;
        return true;
    }
    if (P.empty() && X.empty()) {
        return false;
    }

    std::vector<int> setVector(P.begin(), P.end());
    for (int v : setVector) {
        if (!cliqueFound) {
            std::set<int> neighbors = graph.at(v);
            std::set<int> P_intersection;
            std::set<int> X_intersection;

            R.insert(v);
            setIntersection(P, neighbors, P_intersection);
            setIntersection(X, neighbors, X_intersection);

            bronKerboschRecurse(R, P_intersection, X_intersection, graph, k, cliqueFound);

            R.erase(v);
            P.erase(v);
            X.insert(v);
        }
    }
    return cliqueFound;
}


bool bronKerbosch(const std::map<int, std::set<int>>& graph, int k) {
    if (k > graph.size()) {
        return false;
    }
    std::set<int> R, P, X;
    bool cliqueFound = false;

    for (const auto& entry : graph) {
        P.insert(entry.first);
    }
    bronKerboschRecurse(R, P, X, graph, k, cliqueFound);
    return cliqueFound;
}