#include <vector>
#include <algorithm>

#include "prime_contract.h"

int PrimeCount(int A, int B) {
    if (B < 2) return 0;

    int max_n = B;
    std::vector<bool> is_prime(max_n + 1, true);
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= max_n; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= max_n; j += i) {
                is_prime[j] = false;
            }
        }
    }

    int count = 0;
    for (int i = A; i <= B; ++i) {
        if (i >= 0 && i <= max_n && is_prime[i]) {
            ++count;
        }
    }
    return count;
}