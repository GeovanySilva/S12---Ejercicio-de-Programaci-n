#include "funciones.h"
#include <string.h>
#include <stdlib.h>

static int compararFechaDesc(const void *a, const void *b) {
    const Venta *va = (const Venta*)a;
    const Venta *vb = (const Venta*)b;
    /* Las fechas en formato YYYY-MM-DD comparan lexicográficamente */
    return strcmp(vb->fecha, va->fecha);
}

static char *trim_newline(char *s) {
    char *p = s + strlen(s) - 1;
    while (p >= s && (*p == '\n' || *p == '\r')) { *p = '\0'; --p; }
    return s;
}

void mostrarVentas(void) {
    const char *ruta = "ventas.txt"; /* Archivo esperado en el mismo directorio */
    FILE *f = fopen(ruta, "r");
    if (!f) {
        printf("No se encontró el archivo de ventas ('%s').\n", ruta);
        return;
    }

    Venta *arr = NULL;
    size_t cap = 0, len = 0;
    char linea[512];

    while (fgets(linea, sizeof(linea), f)) {
        trim_newline(linea);
        if (linea[0] == '\0') continue;
        /* Soportar separador '|' o ',' */
        char *s = linea;
        char *tok;

        /* ID */
        tok = strtok(s, "|,");
        if (!tok) continue;
        int id = atoi(tok);

        /* Fecha */
        tok = strtok(NULL, "|,");
        if (!tok) continue;
        char fecha[11] = "0000-00-00";
        strncpy(fecha, tok, sizeof(fecha)-1);
        fecha[10] = '\0';

        /* Monto */
        tok = strtok(NULL, "|,");
        if (!tok) continue;
        double monto = atof(tok);

        /* Detalle */
        tok = strtok(NULL, "\n");
        char detalle[128] = "";
        if (tok) {
            strncpy(detalle, tok, sizeof(detalle)-1);
            detalle[127] = '\0';
        }

        /* Excluir montos 0 */
        if (monto == 0.0) continue;

        if (len + 1 > cap) {
            size_t ncap = cap == 0 ? 16 : cap * 2;
            Venta *tmp = realloc(arr, ncap * sizeof(Venta));
            if (!tmp) { free(arr); fclose(f); printf("Error de memoria.\n"); return; }
            arr = tmp; cap = ncap;
        }

        arr[len].id = id;
        strncpy(arr[len].fecha, fecha, sizeof(arr[len].fecha)-1);
        arr[len].fecha[10] = '\0';
        arr[len].monto = monto;
        strncpy(arr[len].detalle, detalle, sizeof(arr[len].detalle)-1);
        arr[len].detalle[127] = '\0';
        len++;
    }

    fclose(f);

    if (len == 0) {
        printf("No hay ventas para mostrar (o todas tenían monto 0).\n");
        free(arr);
        return;
    }

    /* Ordenar por fecha descendente */
    qsort(arr, len, sizeof(Venta), compararFechaDesc);

    /* Mostrar tabla */
    printf("+----+------------+-----------+------------------------------------------+\n");
    printf("| ID | Fecha      | Monto     | Detalle                                  |\n");
    printf("+----+------------+-----------+------------------------------------------+\n");
    for (size_t i = 0; i < len; ++i) {
        printf("| %2d | %10s | %9.2f | %-40s |\n",
               arr[i].id, arr[i].fecha, arr[i].monto, arr[i].detalle);
    }
    printf("+----+------------+-----------+------------------------------------------+\n");

    free(arr);
}
