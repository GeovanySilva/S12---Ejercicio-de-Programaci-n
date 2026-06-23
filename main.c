#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "funciones.h"

static void limpiarEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int pedirEntero(const char *prompt) {
    int v;
    printf("%s", prompt);
    while (scanf("%d", &v) != 1) {
        limpiarEntrada();
        printf("Entrada inválida. %s", prompt);
    }
    limpiarEntrada();
    return v;
}

static void mostrarMenuPrincipal(void) {
    printf("\n=================================================\n");
        printf("      SGIC - Ruedas de Oro (Menu Principal)\n");
    printf("=================================================\n");
        printf("1) Vehiculos\n");
    printf("2) Clientes\n");
    printf("3) Vendedores\n");
    printf("4) Ventas\n");
    printf("5) Historial de ventas\n");
    printf("6) Salir\n");
    printf("-------------------------------------------------\n");
}

static void menuVehiculos(void) {
    while (1) {
        printf("\n--- Vehiculos ---\n");
            printf("1) Agregar vehiculo\n");
            printf("2) Listar vehiculos\n");
            printf("3) Editar vehiculo\n");
            printf("4) Eliminar vehiculo\n");
            printf("5) Vehiculos vendidos (listar/editar)\n");
            printf("6) Recomendar vehiculo\n");
        printf("0) Volver\n");
            int o = pedirEntero("Elija opcion: ");
        if (o == 0) break;
        switch (o) {
            case 1: agregarVehiculo(); break;
            case 2: listarVehiculos(); break;
            case 3: editarVehiculo(); break;
            case 4: eliminarVehiculo(); break;
            case 5: editarVehiculoVendido(); break;
            case 6: recomendarVehiculo(); break;
            default: printf("Opción inválida.\n"); break;
        }
    }
}

static void menuClientes(void) {
    while (1) {
        printf("\n--- Clientes ---\n");
        printf("1) Agregar cliente\n");
        printf("2) Listar clientes\n");
        printf("3) Editar cliente\n");
        printf("4) Eliminar cliente\n");
        printf("0) Volver\n");
            int o = pedirEntero("Elija opcion: ");
        if (o == 0) break;
        switch (o) {
            case 1: agregarCliente(); break;
            case 2: listarClientes(); break;
            case 3: editarCliente(); break;
            case 4: eliminarCliente(); break;
            default: printf("Opción inválida.\n"); break;
        }
    }
}

static void menuVendedores(void) {
    while (1) {
        printf("\n--- Vendedores ---\n");
        printf("1) Agregar vendedor\n");
        printf("2) Listar vendedores\n");
        printf("3) Editar vendedor\n");
        printf("4) Eliminar vendedor\n");
        printf("0) Volver\n");
            int o = pedirEntero("Elija opcion: ");
        if (o == 0) break;
        switch (o) {
            case 1: agregarVendedor(); break;
            case 2: listarVendedores(); break;
            case 3: editarVendedor(); break;
            case 4: eliminarVendedor(); break;
            default: printf("Opción inválida.\n"); break;
        }
    }
}

static void menuVentas(void) {
    while (1) {
        printf("\n--- Ventas ---\n");
        printf("1) Registrar venta\n");
        printf("2) Mostrar historial de ventas\n");
        printf("0) Volver\n");
            int o = pedirEntero("Elija opcion: ");
        if (o == 0) break;
        switch (o) {
            case 1: registrarVenta(); break;
            case 2: mostrarVentas(); break;
            default: printf("Opción inválida.\n"); break;
        }
    }
}

int main(void) {
    while (1) {
        mostrarMenuPrincipal();
            int opt = pedirEntero("Seleccione una opcion: ");
        switch (opt) {
            case 1: menuVehiculos(); break;
            case 2: menuClientes(); break;
            case 3: menuVendedores(); break;
            case 4: menuVentas(); break;
            case 5: mostrarVentas(); break;
            case 6: printf("Saliendo...\n"); return 0;
            default: printf("Opción no válida.\n"); break;
        }
    }
    return 0;
}


