// parte 2 seq.c — Secuencial con carga desigual
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

int main(){
    double best = 1e99;
    for(int r=0; r<REPS; r++){
        double t0 = omp_get_wtime();
        for(long i=0; i<N; i++){
            fake_work((i % 1000) + 1);
        }
        double t1 = omp_get_wtime();
        if(t1 - t0 < best) best = t1 - t0;
    }
    printf("Secuencial: N=%d time=%.6f s\n", N, best);
    return 0;
}
