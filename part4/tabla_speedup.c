#include <stdio.h>

int main() {
    int p[] = {1, 2, 4, 8, 12};
    double time_s[] = {0.009780, 0.004915, 0.002446, 0.002161, 0.001420};
    int n = sizeof(p) / sizeof(p[0]);

    double T1 = time_s[0]; 

    FILE *fp = fopen("resultados.csv", "w");
    if (!fp) {
        perror("No se pudo crear el archivo CSV");
        return 1;
    }

    fprintf(fp, "p,time_s,speedup,efficiency\n");
    printf("p\ttiempo(s)\tspeedup\teficiencia\n");
    for (int i = 0; i < n; i++) {
        double S = T1 / time_s[i];   
        double E = S / p[i];         
        fprintf(fp, "%d,%.6f,%.3f,%.3f\n", p[i], time_s[i], S, E);
        printf("%d\t%.6f\t%.3f\t%.3f\n", p[i], time_s[i], S, E);
    }

    fclose(fp);
    printf("\nResultados guardados en 'resultados.csv'\n");

    return 0;
}
