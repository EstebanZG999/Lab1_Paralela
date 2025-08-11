#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000
#define REPS 3

int main() {
    int *A = (int*) aligned_alloc(64, N * sizeof(int));
    for (long i = 0; i < N; i++) A[i] = (int)i;

    double best = 1e99;
    long long sum = 0;

    for (int r = 0; r < REPS; r++) {
        long long s = 0;
        double t0 = omp_get_wtime();
        for (long i = 0; i < N; i++) s += A[i];
        double t1 = omp_get_wtime();
        if (t1 - t0 < best) { best = t1 - t0; sum = s; }
    }

    printf("Secuencial -> sum=%lld time=%.6f s\n", sum, best);
    free(A);
    return 0;
}
