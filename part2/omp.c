// parte 2 omp.c — OpenMP: static vs dynamic vs guided con distintos chunk sizes
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#ifndef N
#define N 200000
#endif
#ifndef REPS
#define REPS 3
#endif

static inline void fake_work(long iters){
    double x = 0.0;
    for(long k = 0; k < iters; k++) x += 1.0 / (k + 1.0);
    if (x < 0) printf("anti-optimizer\n");
}

static double test_static(int chunk){
    double best = 1e99;
    for(int r=0; r<REPS; r++){
        double t0 = omp_get_wtime();
        #pragma omp parallel for schedule(static,chunk)
        for(long i=0; i<N; i++) fake_work((i % 1000) + 1);
        double t1 = omp_get_wtime();
        if(t1 - t0 < best) best = t1 - t0;
    }
    return best;
}

static double test_dynamic(int chunk){
    double best = 1e99;
    for(int r=0; r<REPS; r++){
        double t0 = omp_get_wtime();
        #pragma omp parallel for schedule(dynamic,chunk)
        for(long i=0; i<N; i++) fake_work((i % 1000) + 1);
        double t1 = omp_get_wtime();
        if(t1 - t0 < best) best = t1 - t0;
    }
    return best;
}

static double test_guided(int chunk){
    double best = 1e99;
    for(int r=0; r<REPS; r++){
        double t0 = omp_get_wtime();
        #pragma omp parallel for schedule(guided,chunk)
        for(long i=0; i<N; i++) fake_work((i % 1000) + 1);
        double t1 = omp_get_wtime();
        if(t1 - t0 < best) best = t1 - t0;
    }
    return best;
}

int main(){
    int chunks[] = {1, 8, 64, 512, 2048};
    int n_chunks = sizeof(chunks)/sizeof(chunks[0]);

    printf("policy,chunk,N,time_s\n");
    for(int k=0; k<n_chunks; k++){
        int c = chunks[k];
        double ts = test_static(c);
        double td = test_dynamic(c);
        double tg = test_guided(c);
        printf("static,%d,%d,%.6f\n",  c, N, ts);
        printf("dynamic,%d,%d,%.6f\n", c, N, td);
        printf("guided,%d,%d,%.6f\n",  c, N, tg);
    }
    return 0;
}
