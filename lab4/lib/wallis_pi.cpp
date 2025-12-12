#include "pi_contract.h"

float Pi(int K) {
    float pi = 1.0f;

    for (int n = 1; n <= K; ++n) {
        float even = 2.0f * n;   
        float odd1 = 2.0f * n - 1.0f;    
        float odd2 = 2.0f * n + 1.0f;    
        pi *= (even / odd1) * (even / odd2);
    }

    return 2.0f * pi;
}