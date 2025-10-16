#include "sort.h"
#include <thread>
#include <vector>

ParallelMergeSort::ParallelMergeSort(int maxThreads) : maxThreads_(maxThreads) {
    if (maxThreads_ < 1) maxThreads_ = 1;
}

void ParallelMergeSort::Merge(std::vector<int>& arr, int left, int mid, int right) {
    std::vector<int> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<int> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0;
    int j = 0;
    int k = left;

    while (i < static_cast<int>(leftArr.size()) && j < static_cast<int>(rightArr.size())) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k++] = leftArr[i++];
        } else {
            arr[k++] = rightArr[j++];
        }
    }

    while (i < static_cast<int>(leftArr.size())) {
        arr[k++] = leftArr[i++];
    }

    while (j < static_cast<int>(rightArr.size())) {
        arr[k++] = rightArr[j++];
    }
}

void ParallelMergeSort::SequentialSort(std::vector<int>& arr, int left, int right) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;
    SequentialSort(arr, left, mid);
    SequentialSort(arr, mid + 1, right);
    Merge(arr, left, mid, right);
}

void ParallelMergeSort::ParallelSort(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;

    int current = activeThreads_.load();
    bool spawned = false;
    while (current < maxThreads_) {
        if (activeThreads_.compare_exchange_weak(current, current + 1)) {
            std::thread t([this, &arr, left, mid]() {
                ParallelSort(arr, left, mid);
                activeThreads_.fetch_sub(1);
            });
            ParallelSort(arr, mid + 1, right);
            t.join();
            spawned = true;
            break;
        }
    }

    if (spawned) {
        Merge(arr, left, mid, right);
    } else {
        SequentialSort(arr, left, right);
    }
}

void ParallelMergeSort::Sort(std::vector<int>& arr) {
    if (arr.size() <= 1) {
        return;
    }
    activeThreads_.store(1);
    ParallelSort(arr, 0, static_cast<int>(arr.size()) - 1);
}