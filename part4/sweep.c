// Parte 4 — gcc -fopenmp sweep.c -o sweep

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#ifndef N
#define N 10000000
#endif
#ifndef REPS
#define REPS 5
#endif

static inline int is_even(int x){ return (x & 1) == 0; }

int main() {
    int p = 0;
    #pragma omp parallel
    {
        #pragma omp single
        p = omp_get_num_threads();
    }

    double best = 1e99;
    long long sink = 0;
    for (int r = 0; r < REPS; r++) {
        long long cnt = 0;
        double t0 = omp_get_wtime();
        #pragma omp parallel for reduction(+:cnt) schedule(static)
        for (long i = 0; i < N; i++) cnt += is_even((int)i);
        double t1 = omp_get_wtime();
        double dt = t1 - t0;
        if (dt < best) best = dt;
        sink += cnt; // anti-optimizer
    }
    if (sink == -42) fprintf(stderr,".\n"); 

    // Solo imprime: p,time_s
    printf("%d,%.6f\n", p, best);
    return 0;
}