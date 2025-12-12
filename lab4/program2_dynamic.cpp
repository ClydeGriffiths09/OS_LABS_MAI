#include <iostream>
#include <string>
#include <memory>

#include "dynamic_library.h"
#include "prime_contract.h"
#include "pi_contract.h"

int main() {
    std::string prime_path = "./libprime_naive.so";
    std::string pi_path = "./libpi_leibniz.so";

    auto prime_lib = std::make_unique<DynamicLibrary>(prime_path);
    auto pi_lib = std::make_unique<DynamicLibrary>(pi_path);

    auto prime_func = prime_lib->getSymbolAs<int(*)(int, int)>("PrimeCount");
    auto pi_func = pi_lib->getSymbolAs<float(*)(int)>("Pi");

    std::cout << "Program 2: Dynamic loading (Set A: Naive + Leibniz)\n";
    std::cout << "0 - switch set\n1 A B - prime count\n2 K - Pi\n";

    std::string cmd;
    while (std::cin >> cmd) {
        if (cmd == "0") {
            if (prime_path == "./libprime_naive.so") {
                prime_path = "./libprime_erat.so";
                pi_path = "./libpi_wallis.so";
            } else {
                prime_path = "./libprime_naive.so";
                pi_path = "./libpi_leibniz.so";
            }

            try {
                prime_lib = std::make_unique<DynamicLibrary>(prime_path);
                pi_lib = std::make_unique<DynamicLibrary>(pi_path);
                prime_func = prime_lib->getSymbolAs<int(*)(int, int)>("PrimeCount");
                pi_func = pi_lib->getSymbolAs<float(*)(int)>("Pi");
                std::cout << "Switched to: " 
                          << (prime_path == "./libprime_naive.so" ? "Set A" : "Set B") << "\n";
            } catch (const std::exception& e) {
                std::cerr << "Switch failed: " << e.what() << "\n";
            }
        }
        else if (cmd == "1") {
            int A, B; std::cin >> A >> B;
            std::cout << "Primes in [" << A << ", " << B << "] = " << prime_func(A, B) << "\n";
        }
        else if (cmd == "2") {
            int K; std::cin >> K;
            std::cout << "Pi (K=" << K << ") = " << pi_func(K) << "\n";
        }
        else {
            std::cout << "Unknown command\n";
        }
    }

    return 0;
}