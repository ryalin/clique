#include <algorithm>
#include <iostream>
#include <getopt.h>
#include <vector>
#include <set>
#include <map>
#include <mpi.h>

#include "bron-kerbosch.h"
#include "../test.h"
#include "../timing.h"
#include "../util.h"

// Given 2 sets, finds the intersection and populates into result
void setIntersectionP(const std::set<int>& set1, const std::set<int>& set2, 
                     std::set<int>& result) {
    result.clear();
    set_intersection(set1.begin(), set1.end(), set2.begin(), 
                     set2.end(), inserter(result, result.begin()));
}


// Bron-Kerbosch algorithm recursive helper
bool bronKerboschRecurseP(std::set<int>& R, std::set<int>& P, std::set<int>& X, 
                         const std::map<int, std::set<int>>& graph, int k) {
    if (P.empty() && X.empty()) {
        // Check if R is a clique of size k
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

        // Include v in the current clique R
        #pragma omp parallel
        #pragma omp single
        {
        #pragma omp task shared(R)
        R.insert(v);

        #pragma omp task shared(P_intersection)
        setIntersectionP(P, neighbors, P_intersection);

        #pragma omp task shared(X_intersection)
        setIntersectionP(X, neighbors, X_intersection);
        }
        #pragma omp taskwait

        bool res = false;
        #pragma omp task shared(res)
        {
        res = bronKerboschRecurseP(R, P_intersection, X_intersection, graph, k);
        }
        if (res) {
            return true;
        }
        R.erase(v);
        P.erase(v);
        X.insert(v);
    }
    return false;
}


// Parallel implementation of bron-kerbosch algorithm
bool bronKerboschParallel(const std::map<int,std::set<int>>& graph, int k) {
    std::set<int> r;
    std::set<int> p;
    std::set<int> x;

    for (const auto& entry : graph) {
        p.insert(entry.first);
    }
    return bronKerboschRecurseP(r, p, x, graph, k);
}


// Runs parallel bron-kerbosch algorithm against normal version
int main(int argc, char *argv[]) {

    // Run instructions:
    // make run FLAGS="-s <size> -d <density> -t <target>"

    // Required Inputs:
    // -t <size>        size of clique to look for
    // -s <size>        size of graph generated
    // -d <density>     density of graph from 0 to 1

    // Optional:
    // -h               displays help message

    MPI_Init(&argc, &argv);

    int pid, nproc;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    int option;
    int s = 30;
    int t = 10;
    float d = 1;
    bool checkCorrectness = false;
    bool benchmark = false;
    while ((option = getopt(argc, argv, "s:d:t:h")) != -1) {
        switch (option) {
        case 's':
            s = atoi(optarg);
            break;
        case 'd':
            d = std::stof(optarg);
            break;
        case 't':
            t = atoi(optarg);
            break;
        case 'h':
            displayUsage();
            return 0;
        case '?':
            displayUsage();
            return 0;
        default:
            std::cerr << "Unknown option: " << static_cast<char>(optopt) << std::endl;
            displayUsage();
            return 0;
        }
    }
    // Generate random graph with command line size and density
    std::map<int, std::set<int>> graph = generateRandom(s, d);

    Timer parallelTimer;
    bool parRes = bronKerboschParallel(graph, t);
    double parSimTime = parallelTimer.elapsed();

    Timer seqTimer;
    bool seqRes = bronKerbosch(graph, t);
    double seqSimTime = seqTimer.elapsed();

    if (pid == 0) {
        if (parRes != seqRes) {
            std::cout << "Error: Parallel and Sequential Returned Different Values" << std::endl;
            return 0;
        }

        printf("\n------ Test with graph size %d, density %.2f, target %d ------\n", s, d, t);

        std::cout << std::left << "\n" << std::setw(15) << "Sequential" << std::setw(15) 
        << "Parallel" << std::setw(15) << "Speedup" << std::setw(15) 
        << "Contains Clique?" << std::endl;

        std::cout << "--------------------------------------------------------------" << std::endl;

        std::cout << std::left << std::setw(15) << seqSimTime << std::setw(15) 
        << parSimTime << std::setw(15) << seqSimTime / parSimTime << std::setw(15) 
        << std::setw(15) << parRes << "\n" << std::endl;
    }
    MPI_Finalize();
    return 0;
}
