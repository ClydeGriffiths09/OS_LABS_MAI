#include <utils.h>
#include <random>

std::vector<int> Utils::GenerateRandomArray(size_t size) {
    std::vector<int> arr(size);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);
    for (auto& x : arr) x = dis(gen);
    return arr;
}