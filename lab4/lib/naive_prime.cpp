#include <cmath>

#include "prime_contract.h"

bool isPrime(int n) {
    if (n < 2) {
        return false;
    }
    if (n == 2) {
        return true;
    }
    if (n % 2 == 0) {
        return false;
    }
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int PrimeCount(int A, int B) {
    int count = 0;
    for (int i = A; i <= B; ++i) {
        if (isPrime(i)) {
            ++count;
        }
    }
    return count;
}