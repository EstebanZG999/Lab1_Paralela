#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int p[]        = {1, 2, 4, 8, 12};
    double time_s[] = {0.009780, 0.004915, 0.002446, 0.002161, 0.001420};
    int n = (int)(sizeof(p) / sizeof(p[0]));
    double T1 = time_s[0];

    // Escribir CSV con speedup y eficiencia
    FILE *csv = fopen("resultados.csv", "w");
    if (!csv) { perror("No se pudo crear resultados.csv"); return 1; }
    fprintf(csv, "p,time_s,speedup,efficiency\n");

    // También guardamos arrays calculados para gnuplot
    double *speedup = (double*)malloc(n * sizeof(double));
    double *eff     = (double*)malloc(n * sizeof(double));
    if (!speedup || !eff) { fprintf(stderr, "Memoria insuficiente\n"); return 1; }

    for (int i = 0; i < n; i++) {
        speedup[i] = T1 / time_s[i];
        eff[i]     = speedup[i] / p[i];
        fprintf(csv, "%d,%.6f,%.3f,%.3f\n", p[i], time_s[i], speedup[i], eff[i]);
    }
    fclose(csv);

    // Abrir gnuplot por tubería y dibujar SPEEDUP
    FILE *gp = popen("gnuplot -persist", "w");
    if (!gp) { fprintf(stderr, "No se pudo invocar gnuplot. ¿Está instalado?\n"); return 1; }

    // Config común
    fprintf(gp, "set term pngcairo size 1000,650\n");
    fprintf(gp, "set datafile separator ','\n");
    fprintf(gp, "set grid\n");
    fprintf(gp, "set xtics rotate by 0\n");

    // SPEEDUP
    fprintf(gp, "set output 'speedup.png'\n");
    fprintf(gp, "set title 'Speedup vs numero de hilos'\n");
    fprintf(gp, "set xlabel 'Numero de hilos (p)'\n");
    fprintf(gp, "set ylabel 'Speedup (T1/Tp)'\n");
    fprintf(gp, "plot 'resultados.csv' using 1:3 with linespoints lt 1 pt 7 lw 2 title 'Speedup'\n");

    // EFICIENCIA
    fprintf(gp, "set output 'efficiency.png'\n");
    fprintf(gp, "set title 'Eficiencia vs numero de hilos'\n");
    fprintf(gp, "set xlabel 'Numero de hilos (p)'\n");
    fprintf(gp, "set ylabel 'Eficiencia (Speedup/p)'\n");
    fprintf(gp, "plot 'resultados.csv' using 1:4 with linespoints lt 1 pt 7 lw 2 title 'Eficiencia'\n");

    fflush(gp);
    pclose(gp);

    // Imprimir tabla bonita en consola (opcional)
    printf("p\ttiempo(s)\tspeedup\teficiencia\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%.6f\t%.3f\t%.3f\n", p[i], time_s[i], speedup[i], eff[i]);
    }

    free(speedup);
    free(eff);
    printf("\nListo: resultados.csv, speedup.png, efficiency.png\n");
    return 0;
}
