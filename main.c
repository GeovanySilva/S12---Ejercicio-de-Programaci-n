#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

void mostrarMenu(void) {
    printf("\n=== MENU PRINCIPAL ===\n");
    printf("1. Opción 1 (no implementado)\n");
    printf("2. Opción 2 (no implementado)\n");
    printf("3. Opción 3 (no implementado)\n");
    printf("4. Opción 4 (no implementado)\n");
    printf("5. Opción 5 (no implementado)\n");
    printf("6. Opción 6 (no implementado)\n");
    printf("7. Mostrar historial de ventas\n");
    printf("8. Salir\n");
    printf("Seleccione una opción: ");
}

int main(void) {
    int opt = 0;
    while (1) {
        mostrarMenu();
        if (scanf("%d", &opt) != 1) { /* limpiar entrada */
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            continue;
        }
        switch (opt) {
            case 7:
                mostrarVentas();
                break;
            case 8:
                printf("Saliendo...\n");
                return 0;
            default:
                printf("Opción no implementada en esta copia demo.\n");
                break;
        }
    }
    return 0;
}

