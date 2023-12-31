#include <algorithm>
#include <iostream>
#include <getopt.h>
#include <vector>
#include <set>
#include <map>
#include <mpi.h>
#include <omp.h>

#include "bron-kerbosch.h"
#include "../test.h"
#include "../timing.h"
#include "../util.h"


int** mapToAdjacencyMatrix(std::map<int, std::set<int>>& graph, int size) {
    // Initialize the matrix with zeros
    int** adjacencyMatrix = new int*[size];
    for (int i = 0; i < size; ++i) {
        adjacencyMatrix[i] = new int[size]();
    }
    // Fill in the matrix based on the graph
    for (const auto& entry : graph) {
        int node = entry.first;
        for (int neighbor : entry.second) {
            adjacencyMatrix[node][neighbor] = 1;
            adjacencyMatrix[neighbor][node] = 1;
        }
    }
    return adjacencyMatrix;
}


std::map<int, std::set<int>> adjacencyMatrixToMap(int** adjacencyMatrix, int size) {
    std::map<int, std::set<int>> graph;

    // Iterate through the rows and columns of the matrix
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            // If there is an edge (matrix[i][j] is non-zero), add it to the map
            if (adjacencyMatrix[i][j] != 0) {
                graph[i].insert(j);
                graph[j].insert(i);
            }
        }
    }
    return graph;
}


bool bronKerboschRecurseP(std::set<int>& R, std::set<int>& P, std::set<int>& X, 
                         const std::map<int, std::set<int>>& graph, int k, bool& cliqueFound) {
    if (R.size() >= k) {
        cliqueFound = true;
        return true;
    }
    if (P.empty() && X.empty()) {
        return false;
    }
    std::vector<int> setVector(P.begin(), P.end());

    #pragma omp parallel
    #pragma omp single nowait
    for (int v : setVector) {
        if (!cliqueFound) {
            std::set<int> neighbors = graph.at(v);
            std::set<int> P_intersection;
            std::set<int> X_intersection;

            R.insert(v);
            setIntersection(P, neighbors, P_intersection);
            setIntersection(X, neighbors, X_intersection);

            #pragma omp task firstprivate(P_intersection, X_intersection, R) shared(cliqueFound)
            bronKerboschRecurseP(R, P_intersection, X_intersection, graph, k, cliqueFound);

            R.erase(v);
            P.erase(v);
            X.insert(v);
        }
    }
    #pragma omp taskwait
    return cliqueFound;
}

bool bronKerboschParallel(const std::map<int, std::set<int>>& graph, int k) {
    if (k > graph.size()) {
        return false;
    }
    std::set<int> R, P, X;
    bool cliqueFound = false;

    for (const auto& entry : graph) {
        P.insert(entry.first);
    }

    bronKerboschRecurseP(R, P, X, graph, k, cliqueFound);
    return cliqueFound;
}


// Runs parallel bron-kerbosch algorithm against normal version
int main(int argc, char *argv[]) {

    // Run instructions:
    // make run FLAGS="-s <size> -d <density> -t <target> -h -c"

    // Inputs:
    // -t <size>        size of clique to look for
    // -s <size>        size of graph generated
    // -d <density>     density of graph from 0 to 1

    // Optional:
    // -h               displays help message
    // -c               correctness check with test cases

    MPI_Init(&argc, &argv);

    int pid, nproc;
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &nproc);

    int option;
    int s = 30;
    int t = 10;
    float d = 1;
    bool checkCorrectness = false;
    while ((option = getopt(argc, argv, "cs:d:t:h")) != -1) {
        switch (option) {
        case 's':
            s = atoi(optarg);
            break;
        case 'c':
            checkCorrectness = true;
            break;
        case 'd':
            d = std::stof(optarg);
            break;
        case 't':
            t = atoi(optarg);
            break;
        case 'h':
            displayUsage();
            MPI_Finalize();
            return 0;
        case '?':
            displayUsage();
            MPI_Finalize();
            return 0;
        default:
            std::cerr << "Unknown option: " << static_cast<char>(optopt) << std::endl;
            displayUsage();
            MPI_Finalize();
            return 0;
        }
    }

    if (checkCorrectness) {
        std::vector<Graph> tests;
        tests.push_back({4, "one_clique5", readGraphFromTxt("../tests/one_clique5.txt")});
        tests.push_back({14, "one_clique15", readGraphFromTxt("../tests/one_clique15.txt")});
        tests.push_back({29, "one_clique30", readGraphFromTxt("../tests/one_clique30.txt")});

        if (pid == 0) {
        std::cout << "---------------- Correctness Test (BRONK) -----------------" << std::endl;
        }

        for (auto graph: tests) {
            if (pid == 0) {
                std::cout << "Running test " << graph.name << std::endl;
            }

            // Check if clique is in graph
            bool res1 = bronKerboschParallel(graph.nodes, graph.targetSize);
            bool res2 = bronKerbosch(graph.nodes, graph.targetSize);
            if ((res1 != res2) && (pid == 0)) {
                std::cerr << "Error: sequential and parallel returned different results" << std::endl;
                MPI_Finalize();
                return 0;
            }
            if ((res1 == 0) && (pid == 0)) {
                std::cerr << "Error: output should be true" << std::endl;
                MPI_Finalize();
                return 0;
            }
            // Check if target size clique is non-existent
            res1 = bronKerboschParallel(graph.nodes, graph.targetSize + 2);
            res2 = bronKerbosch(graph.nodes, graph.targetSize + 2);
            if ((res1 != res2) && (pid == 0)) {
                std::cerr << "Error: sequential and parallel returned different results" << std::endl;
                MPI_Finalize();
                return 0;
            }
            if ((res1 == 1) && (pid == 0)) {
                std::cerr << "Error: output should be false" << std::endl;
                MPI_Finalize();
                return 0;
            }
        }
        Graph multi;
        multi.nodes = readGraphFromTxt("../tests/multi_clique.txt");
        for (int i = 1; i < 4; i++) {
            if (pid == 0) {
                std::cout << "Running test multi_clique" << i * 10 << std::endl;
            }
            bool res1 = bronKerboschParallel(multi.nodes, i * 10);
            bool res2 = bronKerbosch(multi.nodes, i * 10);
            if ((res1 != res2) && (pid == 0)) {
                std::cerr << "Error: sequential and parallel returned different results" << std::endl;
                MPI_Finalize();
                return 0;
            }
            if ((res1 == 0) && (pid == 0)) {
                std::cerr << "Error: output should be true" << std::endl;
                MPI_Finalize();
                return 0;
            }
        }
        if (pid == 0) {
            std::cout << "All tests passed!" << std::endl;
        }
        MPI_Finalize();
        return 0;
    }

    // Generate random graph with command line size and density
    // and send to other processes. We are not implementing MPI for
    // Bron-Kerbosch currently though.
    std::map<int, std::set<int>> graph;
    int** matrix;
    if (pid == 0) {
        printf("\n------ Test with graph size %d, density %.4f, target %d ------\n", s, d, t);
        graph = generateRandom(s, d);
        matrix = mapToAdjacencyMatrix(graph, s);
        for (int receiver = 1; receiver < nproc; receiver++) {
            for (int row = 0; row < s; row++) {
                MPI_Send(matrix[row], s, MPI_INT, receiver, row, MPI_COMM_WORLD);
            }
        }
    } else {
        matrix = new int*[s];
        for (int row = 0; row < s; row++) {
            matrix[row] = new int[s];
            MPI_Recv(matrix[row], s, MPI_INT, 0, row, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        graph = adjacencyMatrixToMap(matrix, s);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (pid == 0) {        
        Timer parallelTimer;
        bool parRes = bronKerboschParallel(graph, t);
        double parSimTime = parallelTimer.elapsed();

        Timer seqTimer;
        bool seqRes = bronKerbosch(graph, t);
        double seqSimTime = seqTimer.elapsed();

        if (parRes != seqRes) {
            std::cout << "Error: Parallel and Sequential Returned Different Values" << std::endl;
            MPI_Finalize();
            return 0;
        }

        std::cout << std::left << "\n" << std::setw(15) << "Sequential" 
        << std::setw(15) << "Parallel" << std::setw(15) << "Speedup" 
        << std::setw(15) << "Contains Clique?" << std::endl;

        std::cout << "--------------------------------------------------------------" << std::endl;

        std::cout << std::left << std::setw(15) << seqSimTime << std::setw(15) 
        << parSimTime << std::setw(15) << seqSimTime / parSimTime << std::setw(15)
        << std::setw(15) << parRes << "\n" << std::endl;
    }
    MPI_Finalize();
    return 0;
}
