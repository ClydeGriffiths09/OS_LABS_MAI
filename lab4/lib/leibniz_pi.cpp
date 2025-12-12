#include "pi_contract.h"

float Pi(int K) {
    float pi = 0.0f;

    for (int i = 0; i < K; ++i) {
        float term = 1.0f / (2 * i + 1);
        if (i % 2 == 1) {
            term = -term;
        }
        pi += term;
    }
    
    return pi * 4.0f;
}