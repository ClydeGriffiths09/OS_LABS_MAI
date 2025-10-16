#pragma once

#include <vector>
#include <atomic>

class ParallelMergeSort {
public:
    explicit ParallelMergeSort(int maxThreads);
    void Sort(std::vector<int>& arr);

private:
    void Merge(std::vector<int>& arr, int left, int mid, int right);
    void SequentialSort(std::vector<int>& arr, int left, int right);
    void ParallelSort(std::vector<int>& arr, int left, int right);
    
    int maxThreads_;
    std::atomic<int> activeThreads_{1};
};
