# S12---Ejercicio-de-Programaci-n

# INSTRUCCIONES.md

## Objetivo

Desarrollar en lenguaje C un Sistema de Gestión de Inventario y Clientes (SGIC) para la concesionaria "Ruedas de Oro", utilizando programación modular y persistencia de datos mediante archivos.

## Requisitos Generales

* Utilizar lenguaje C.
* Aplicar programación modular.
* Utilizar estructuras (`struct`).
* Implementar funciones para cada proceso.
* Guardar y recuperar información desde archivos.
* El sistema debe ejecutarse mediante un menú principal.

## Estructura del Proyecto

El proyecto debe estar dividido en tres archivos principales:

### main.c

Responsabilidades:

* Mostrar el menú principal.
* Gestionar la interacción con el usuario.
* Llamar a las funciones de los demás módulos.
* Controlar el flujo del programa.

### funciones.h

Responsabilidades:

* Declarar estructuras.
* Declarar prototipos de funciones.
* Compartir definiciones entre archivos.

### funciones.c

Responsabilidades:

* Implementar todas las funcionalidades del sistema.
* Gestionar archivos.
* Gestionar vehículos.
* Gestionar clientes.
* Gestionar ventas.

## Funcionalidades Obligatorias

### Gestión de Vehículos

* Registrar vehículo.
* Consultar vehículo.
* Buscar vehículo por marca.
* Buscar vehículo por tipo.
* Buscar vehículo por rango de precio.
* Modificar vehículo.
* Eliminar vehículo.
* Mostrar inventario completo.

### Gestión de Clientes

* Registrar cliente.
* Consultar cliente.
* Buscar cliente.

### Gestión de Ventas

* Registrar venta.
* Mostrar historial de ventas.
* Actualizar inventario después de una venta.

### Persistencia

Toda la información debe almacenarse en archivos para que no se pierda al cerrar el programa.

Archivos sugeridos:

* vehiculos.dat
* clientes.dat
* ventas.dat

## Caso de Prueba

El sistema debe permitir resolver el siguiente escenario:

Un cliente llamado Rubén busca una camioneta Chevrolet usada con un presupuesto máximo de $14.000.

El sistema debe:

1. Buscar vehículos que cumplan esos criterios.
2. Mostrar las opciones disponibles.
3. Permitir seleccionar un vehículo.
4. Registrar la venta.
5. Guardar la venta en archivo.

## Menú Sugerido

1. Registrar vehículo
2. Mostrar vehículos
3. Buscar vehículo
4. Registrar cliente
5. Registrar venta
6. Mostrar inventario
7. Mostrar historial de ventas
8. Salir

## Entregables

El proyecto debe incluir:

* main.c
* funciones.c
* funciones.h
* ANALISIS_FUNCIONAL.md
* PLAN.md
* INSTRUCCIONES.md

El código debe compilar correctamente utilizando GCC sin errores.
