#include <iostream>
#include <string>

#include "prime_contract.h"
#include "pi_contract.h"

int main() {
    std::cout << "Program 1 (Set A): Naive + Leibniz (static linking)\n";
    std::cout << "0 - switch (not available)\n1 A B - prime count\n2 K - Pi\n";

    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "0") {
            std::cout << "Switch not available in static linkage.\n";
        } else if (cmd == "1") {
            int A, B; std::cin >> A >> B;
            std::cout << "Primes: " << PrimeCount(A, B) << "\n";
        } else if (cmd == "2") {
            int K; std::cin >> K;
            std::cout << "Pi: " << Pi(K) << "\n";
        }
    }
    return 0;
}