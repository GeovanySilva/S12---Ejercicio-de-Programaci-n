#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Vehiculo{
int codigo;
char marca[30];
char modelo[30];
char tipo[20];
int anio;
float precio;
int disponible;
};

struct Cliente{
int codigo;
char nombre[50];
int edad;
};

void registrarVehiculo(){
FILE *arch;
struct Vehiculo v;

```
arch = fopen("vehiculos.txt","a");

if(arch == NULL){
    printf("Error al abrir archivo\n");
    return;
}

printf("\nREGISTRO DE VEHICULO\n");

printf("Codigo: ");
scanf("%d",&v.codigo);

printf("Marca: ");
scanf("%s",v.marca);

printf("Modelo: ");
scanf("%s",v.modelo);

printf("Tipo: ");
scanf("%s",v.tipo);

printf("Anio: ");
scanf("%d",&v.anio);

printf("Precio: ");
scanf("%f",&v.precio);

v.disponible = 1;

fprintf(arch,"%d %s %s %s %d %.2f %d\n",
        v.codigo,
        v.marca,
        v.modelo,
        v.tipo,
        v.anio,
        v.precio,
        v.disponible);

fclose(arch);

printf("\nVehiculo guardado correctamente\n");
```

}

void mostrarVehiculos(){

```
FILE *arch;
struct Vehiculo v;

arch = fopen("vehiculos.txt","r");

if(arch == NULL){
    printf("\nNo hay vehiculos registrados\n");
    return;
}

printf("\nLISTA DE VEHICULOS\n\n");

while(fscanf(arch,"%d %s %s %s %d %f %d",
      &v.codigo,
      v.marca,
      v.modelo,
      v.tipo,
      &v.anio,
      &v.precio,
      &v.disponible) != EOF){

    printf("Codigo: %d\n",v.codigo);
    printf("Marca: %s\n",v.marca);
    printf("Modelo: %s\n",v.modelo);
    printf("Tipo: %s\n",v.tipo);
    printf("Anio: %d\n",v.anio);
    printf("Precio: %.2f\n",v.precio);

    if(v.disponible==1)
        printf("Estado: Disponible\n");
    else
        printf("Estado: Vendido\n");

    printf("---------------------------\n");
}

fclose(arch);
```

}

void buscarVehiculo(){

```
FILE *arch;
struct Vehiculo v;

char marca[30];
char tipo[20];
float presupuesto;

int encontrado = 0;

arch = fopen("vehiculos.txt","r");

if(arch == NULL){
    printf("No hay registros\n");
    return;
}

printf("\nBUSQUEDA DE VEHICULOS\n");

printf("Marca: ");
scanf("%s",marca);

printf("Tipo: ");
scanf("%s",tipo);

printf("Presupuesto maximo: ");
scanf("%f",&presupuesto);

printf("\nResultados:\n\n");

while(fscanf(arch,"%d %s %s %s %d %f %d",
      &v.codigo,
      v.marca,
      v.modelo,
      v.tipo,
      &v.anio,
      &v.precio,
      &v.disponible) != EOF){

    if(strcmp(v.marca,marca)==0 &&
       strcmp(v.tipo,tipo)==0 &&
       v.precio <= presupuesto &&
       v.disponible == 1){

        printf("Codigo: %d\n",v.codigo);
        printf("Modelo: %s\n",v.modelo);
        printf("Precio: %.2f\n",v.precio);
        printf("-----------------\n");

        encontrado = 1;
    }
}

if(encontrado == 0)
    printf("No se encontraron vehiculos\n");

fclose(arch);
```

}

void registrarCliente(){

```
FILE *arch;
struct Cliente c;

arch = fopen("clientes.txt","a");

if(arch == NULL){
    printf("Error\n");
    return;
}

printf("\nREGISTRO DE CLIENTE\n");

printf("Codigo: ");
scanf("%d",&c.codigo);

printf("Nombre: ");
scanf("%s",c.nombre);

printf("Edad: ");
scanf("%d",&c.edad);

fprintf(arch,"%d %s %d\n",
        c.codigo,
        c.nombre,
        c.edad);

fclose(arch);

printf("Cliente registrado correctamente\n");
```

}

void registrarVenta(){

```
FILE *archVentas;

int codigoVehiculo;
int codigoCliente;

archVentas = fopen("ventas.txt","a");

if(archVentas == NULL){
    printf("Error\n");
    return;
}

printf("\nREGISTRO DE VENTA\n");

printf("Codigo del cliente: ");
scanf("%d",&codigoCliente);

printf("Codigo del vehiculo: ");
scanf("%d",&codigoVehiculo);

fprintf(archVentas,"%d %d\n",
        codigoCliente,
        codigoVehiculo);

fclose(archVentas);

printf("Venta registrada correctamente\n");
```

}

void mostrarVentas(){

```
FILE *arch;

int cliente;
int vehiculo;

arch = fopen("ventas.txt","r");

if(arch == NULL){
    printf("No existen ventas\n");
    return;
}

printf("\nVENTAS REGISTRADAS\n\n");

while(fscanf(arch,"%d %d",&cliente,&vehiculo) != EOF){

    printf("Cliente: %d\n",cliente);
    printf("Vehiculo: %d\n",vehiculo);
    printf("------------------\n");
}

fclose(arch);
```

}

int main(){

```
int op;

do{

    printf("\n");
    printf("===================================\n");
    printf(" CONCESIONARIA RUEDAS DE ORO\n");
    printf("===================================\n");
    printf("1. Registrar vehiculo\n");
    printf("2. Mostrar inventario\n");
    printf("3. Buscar vehiculo\n");
    printf("4. Registrar cliente\n");
    printf("5. Registrar venta\n");
    printf("6. Mostrar ventas\n");
    printf("7. Salir\n");
    printf("Opcion: ");
    scanf("%d",&op);

    switch(op){

        case 1:
            registrarVehiculo();
            break;

        case 2:
            mostrarVehiculos();
            break;

        case 3:
            buscarVehiculo();
            break;

        case 4:
            registrarCliente();
            break;

        case 5:
            registrarVenta();
            break;

        case 6:
            mostrarVentas();
            break;

        case 7:
            printf("\nPrograma finalizado\n");
            break;

        default:
            printf("\nOpcion incorrecta\n");
    }

}while(op != 7);

return 0;
```

}
