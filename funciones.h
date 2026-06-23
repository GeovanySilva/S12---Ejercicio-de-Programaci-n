#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>

/* Estructura de una venta */
typedef struct {
    int id;
    char fecha[11]; /* YYYY-MM-DD */
    double monto;
    char detalle[128];
} Venta;

/* Muestra el historial de ventas en formato tabla.
   Excluye ventas con monto 0 y ordena por fecha descendente. */
void mostrarVentas(void);

#endif /* FUNCIONES_H */
