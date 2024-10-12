#include <math.h>

int factorial(int n);

double erlang_b(double offered_load, int num_channels) {
    double numerator = pow(offered_load, num_channels) / factorial(num_channels);
    double denominator = 0;
    
    for (int i = 0; i <= num_channels; i++) {
        denominator += pow(offered_load, i) / factorial(i);
    }
    
    return numerator / denominator;
}

int factorial(int n) {
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1);
}