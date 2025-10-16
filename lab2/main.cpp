#include <iostream>
#include <vector>
#include <chrono>
#include <string>
#include <cstdlib>
#include "sort.h"
#include "utils.h"

void PrintUsage(const char* prog) {
    std::cerr << "Usage: " << prog << " --max-threads N\n";
    std::cerr << "  N must be a positive integer (e.g., 1, 2, 4, 8).\n";
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        PrintUsage(argv[0]);
        return 1;
    }

    int maxThreads = -1;

    if (std::string(argv[1]) == "--max-threads") {
        maxThreads = std::atoi(argv[2]);
    } else {
        std::cerr << "Error: expected --max-threads as first argument.\n";
        PrintUsage(argv[0]);
        return 1;
    }

    if (maxThreads <= 0) {
        std::cerr << "Error: --max-threads must be a positive integer.\n";
        return 1;
    }

    const size_t ARRAY_SIZE = 10000000;
    std::cout << "Generating random array of " << ARRAY_SIZE << " elements...\n";
    std::vector<int> arr = Utils::GenerateRandomArray(ARRAY_SIZE);

    std::cout << "Max threads allowed: " << maxThreads << "\n";
    auto start = std::chrono::high_resolution_clock::now();
    ParallelMergeSort sorter(maxThreads);
    sorter.Sort(arr);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Sorting completed in " << duration.count() << " ms\n";

    return 0;
}