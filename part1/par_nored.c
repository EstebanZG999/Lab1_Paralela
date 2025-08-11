#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 100000000
#define REPS 3

int main() {
    int *A = (int*) aligned_alloc(64, N * sizeof(int));
    for (long i = 0; i < N; i++) A[i] = (int)i;

    double best = 1e99;
    long long sum_best = 0;

    for (int r = 0; r < REPS; r++) {
        long long sum = 0;
        double t0 = omp_get_wtime();
        #pragma omp parallel
        {
            long long local = 0;
            #pragma omp for schedule(static)
            for (long i = 0; i < N; i++) local += A[i];
            #pragma omp critical
            { sum += local; }
        }
        double t1 = omp_get_wtime();
        if (t1 - t0 < best) { best = t1 - t0; sum_best = sum; }
    }

    printf("Paralela (sin reduction) -> sum=%lld time=%.6f s\n", sum_best, best);
    free(A);
    return 0;
}
