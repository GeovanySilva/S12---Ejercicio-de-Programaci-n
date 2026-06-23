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

/* Estructuras para vehiculos, clientes y vendedores */
typedef struct {
    int id;
    char placa[16];
    char marca[32];
    char modelo[32];
    char tipo[32];
    int anio;
    char estado[16]; /* Nuevo/Usado */
    double precio;
    int disponible; /* 1 = disponible, 0 = vendido */
} Vehiculo;

typedef struct {
    int id;
    char cedula[32];
    char nombre[64];
    char telefono[32];
    char correo[64];
    char direccion[128];
} Cliente;

typedef struct {
    int id;
    char codigo[32];
    char nombre[64];
    char telefono[32];
    char correo[64];
    double comision; /* acumulada */
} Vendedor;

/* Venta extendida para persistencia */
typedef struct {
    int id;
    char fecha[11]; /* YYYY-MM-DD */
    char cliente[64];
    char vendedor[64];
    char vehiculo[96];
    double precio;
} VentaExt;

/* Funciones: vehiculos */
void cargarVehiculos(Vehiculo **arr, size_t *len);
void guardarVehiculos(Vehiculo *arr, size_t len);
void agregarVehiculo(void);
void listarVehiculos(void);
void listarVehiculosDisponibles(void);
void editarVehiculo(void);
void eliminarVehiculo(void);

/* Funciones: clientes */
void cargarClientes(Cliente **arr, size_t *len);
void guardarClientes(Cliente *arr, size_t len);
void agregarCliente(void);
void listarClientes(void);
void editarCliente(void);
void eliminarCliente(void);

/* Funciones: vendedores */
void cargarVendedores(Vendedor **arr, size_t *len);
void guardarVendedores(Vendedor *arr, size_t len);
void agregarVendedor(void);
void listarVendedores(void);
void editarVendedor(void);
void eliminarVendedor(void);

/* Ventas */
void registrarVenta(void);
void mostrarVentas(void);
void listarVehiculosVendidos(void);
void editarVehiculoVendido(void);
/* Recomendador */
void recomendarVehiculo(void);

#endif /* FUNCIONES_H */
