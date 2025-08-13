// Parte 3: gcc -O3 -march=native -fopenmp p3_bench.c -o p3_bench


#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <stdint.h>

#ifndef N
#define N 10000000
#endif

#ifndef REPS
#define REPS 5
#endif

static inline int is_even(int x) { return (x & 1) == 0; }

static double run_seq(const int *A, long long *out_cnt) {
    double best = 1e99;
    long long best_cnt = 0;
    for (int r = 0; r < REPS; r++) {
        long long cnt = 0;
        double t0 = omp_get_wtime();
        for (long i = 0; i < N; i++) cnt += is_even(A[i]);
        double t1 = omp_get_wtime();
        if (t1 - t0 < best) { best = t1 - t0; best_cnt = cnt; }
    }
    *out_cnt = best_cnt;
    return best;
}

static double run_par_critical(const int *A, long long *out_cnt) {
    double best = 1e99;
    long long best_cnt = 0;
    for (int r = 0; r < REPS; r++) {
        long long cnt = 0;
        double t0 = omp_get_wtime();
        #pragma omp parallel
        {
            long long local = 0;
            #pragma omp for schedule(static)
            for (long i = 0; i < N; i++) local += is_even(A[i]);
            #pragma omp critical
            { cnt += local; }
        }
        double t1 = omp_get_wtime();
        if (t1 - t0 < best) { best = t1 - t0; best_cnt = cnt; }
    }
    *out_cnt = best_cnt;
    return best;
}

static double run_par_reduction(const int *A, long long *out_cnt) {
    double best = 1e99;
    long long best_cnt = 0;
    for (int r = 0; r < REPS; r++) {
        long long cnt = 0;
        double t0 = omp_get_wtime();
        #pragma omp parallel for reduction(+:cnt) schedule(static)
        for (long i = 0; i < N; i++) cnt += is_even(A[i]);
        double t1 = omp_get_wtime();
        if (t1 - t0 < best) { best = t1 - t0; best_cnt = cnt; }
    }
    *out_cnt = best_cnt;
    return best;
}

int main(void) {
    int *A = (int*) malloc(sizeof(int) * (size_t)N);
    if (!A) { fprintf(stderr, "malloc fail\n"); return 1; }
    for (long i = 0; i < N; i++) A[i] = (int)i; 
    int p = omp_get_max_threads();

    long long cseq=0, ccrit=0, cred=0;
    double Tseq = run_seq(A, &cseq);
    double Tcrit = run_par_critical(A, &ccrit);
    double Tred  = run_par_reduction(A, &cred);

    // sanity check
    if (!(cseq == ccrit && ccrit == cred)) {
        fprintf(stderr, "ERROR: resultados distintos: seq=%lld crit=%lld red=%lld\n", cseq, ccrit, cred);
    }

    double Scrit = Tseq / Tcrit;
    double Sred  = Tseq / Tred;
    double Ecrit = Scrit / p;
    double Ered  = Sred  / p;

    printf("N=%d, REPS=%d, threads=%d\n", N, REPS, p);
    printf("mode,time_s,count,speedup,efficiency\n");
    printf("sequential,%.6f,%lld,1.000,%.3f\n", Tseq, cseq, 1.0/p);
    printf("parallel_critical,%.6f,%lld,%.3f,%.3f\n", Tcrit, ccrit, Scrit, Ecrit);
    printf("parallel_reduction,%.6f,%lld,%.3f,%.3f\n", Tred,  cred,  Sred,  Ered);

    free(A);
    return 0;
}
