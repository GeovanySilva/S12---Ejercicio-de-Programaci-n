#include "funciones.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define COMMISSION_RATE 0.05 /* 5% commission */

static char *trim_newline(char *s) {
    if (!s) return s;
    char *p = s + strlen(s) - 1;
    while (p >= s && (*p == '\n' || *p == '\r')) { *p = '\0'; --p; }
    return s;
}

static void limpiarEntradaLocal(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* Helpers de entrada seguros */
static void pedirCadenaLocal(const char *prompt, char *buf, size_t n, int allowDigits, int allowSpaces, int allowPunct) {
    char linea[512];
    while (1) {
        if (prompt) printf("%s", prompt);
        if (!fgets(linea, sizeof(linea), stdin)) { buf[0]='\0'; return; }
        trim_newline(linea);
        if (linea[0] == '\0') { printf("Entrada vacia, intente de nuevo.\n"); continue; }
        int ok = 1;
        for (size_t i=0; linea[i]; ++i) {
            unsigned char ch = (unsigned char)linea[i];
            if (allowSpaces && ch==' ') continue;
            if (allowDigits && (ch>='0' && ch<='9')) continue;
            if (isalpha(ch)) continue;
            if (allowPunct && (ch=='-' || ch=='_' || ch=='.' || ch=='@' || ch=='/' || ch==',')) continue;
            ok = 0; break;
        }
        if (!ok) { printf("Entrada invalida, caracteres no permitidos.\n"); continue; }
        strncpy(buf, linea, n-1); buf[n-1]='\0'; return;
    }
}

static int pedirEnteroLocal(const char *prompt) {
    char linea[128]; long val; char *end;
    while (1) {
        if (prompt) printf("%s", prompt);
        if (!fgets(linea, sizeof(linea), stdin)) return 0;
        trim_newline(linea);
        if (linea[0]=='\0') { printf("Entrada vacia, intente de nuevo.\n"); continue; }
        val = strtol(linea, &end, 10);
        if (*end != '\0') { printf("Ingrese solo numeros enteros.\n"); continue; }
        return (int)val;
    }
}

static double pedirDoubleLocal(const char *prompt) {
    char linea[128]; double val; char *end;
    while (1) {
        if (prompt) printf("%s", prompt);
        if (!fgets(linea, sizeof(linea), stdin)) return 0.0;
        trim_newline(linea);
        if (linea[0]=='\0') { printf("Entrada vacia, intente de nuevo.\n"); continue; }
        val = strtod(linea, &end);
        if (*end != '\0') { printf("Ingrese un numero valido (decimales con punto).\n"); continue; }
        return val;
    }
}

/* ---------- Vehiculos (archivo: vehiculos.dat) ---------- */
void cargarVehiculos(Vehiculo **arr, size_t *len) {
    *arr = NULL; *len = 0;
    FILE *f = fopen("vehiculos.dat", "r");
    if (!f) return;
    char linea[512];
    size_t cap = 0;
    while (fgets(linea, sizeof(linea), f)) {
        trim_newline(linea);
        if (linea[0] == '\0') continue;
        char *tok = strtok(linea, "|,");
        if (!tok) continue;
        Vehiculo v = {0};
        v.id = atoi(tok);
        tok = strtok(NULL, "|,"); if (!tok) continue; strncpy(v.placa, tok, sizeof(v.placa)-1);
        tok = strtok(NULL, "|,"); if (!tok) continue; strncpy(v.marca, tok, sizeof(v.marca)-1);
        tok = strtok(NULL, "|,"); if (!tok) continue; strncpy(v.modelo, tok, sizeof(v.modelo)-1);
        tok = strtok(NULL, "|,"); if (!tok) continue; strncpy(v.tipo, tok, sizeof(v.tipo)-1);
        tok = strtok(NULL, "|,"); if (!tok) continue; v.anio = atoi(tok);
        tok = strtok(NULL, "|,"); if (!tok) continue; strncpy(v.estado, tok, sizeof(v.estado)-1);
        tok = strtok(NULL, "|,"); if (!tok) continue; v.precio = atof(tok);
        tok = strtok(NULL, "|,"); if (!tok) continue; v.disponible = atoi(tok);
        if (*len + 1 > cap) {
            size_t ncap = cap==0?16:cap*2;
            Vehiculo *tmp = realloc(*arr, ncap*sizeof(Vehiculo)); if (!tmp) { free(*arr); *arr = NULL; *len = 0; fclose(f); return; }
            *arr = tmp; cap = ncap;
        }
        (*arr)[(*len)++] = v;
    }
    fclose(f);
}

void guardarVehiculos(Vehiculo *arr, size_t len) {
    FILE *f = fopen("vehiculos.dat", "w");
    if (!f) return;
    /* Solo persistir vehiculos disponibles (disponible == 1) para no mantener vendidos en inventario */
    for (size_t i = 0; i < len; i++) {
        if (arr[i].disponible) {
            fprintf(f, "%d|%s|%s|%s|%s|%d|%s|%.2f|%d\n",
                    arr[i].id, arr[i].placa, arr[i].marca, arr[i].modelo, arr[i].tipo,
                    arr[i].anio, arr[i].estado, arr[i].precio, arr[i].disponible);
        }
    }
    fclose(f);
}

void agregarVehiculo(void) {
    Vehiculo *arr = NULL; size_t len = 0;
    cargarVehiculos(&arr, &len);
    Vehiculo v = {0};
    v.id = (len==0)?1:arr[len-1].id+1;
    pedirCadenaLocal("Placa: ", v.placa, sizeof(v.placa), 1, 0, 1);
    pedirCadenaLocal("Marca: ", v.marca, sizeof(v.marca), 0, 1, 0);
    pedirCadenaLocal("Modelo: ", v.modelo, sizeof(v.modelo), 0, 1, 0);
    pedirCadenaLocal("Tipo: ", v.tipo, sizeof(v.tipo), 0, 1, 0);
    v.anio = pedirEnteroLocal("Ano: ");
    pedirCadenaLocal("Estado (Nuevo/Usado): ", v.estado, sizeof(v.estado), 0, 0, 0);
    v.precio = pedirDoubleLocal("Precio: ");
    v.disponible = 1;
    Vehiculo *tmp = realloc(arr, (len+1)*sizeof(Vehiculo));
    if (!tmp) { free(arr); printf("Error memoria\n"); return; }
    arr = tmp; arr[len++] = v; guardarVehiculos(arr, len); free(arr);
    printf("Vehiculo agregado.\n");
}

void listarVehiculos(void) {
    Vehiculo *arr = NULL; size_t len = 0; cargarVehiculos(&arr, &len);
    printf("+----+-----------+---------------+-----------+------+-------+--------+-----------+\n");
    printf("| ID | Placa     | Marca         | Modelo    | Tipo | Ano   | Estado | Precio    |\n");
    printf("+----+-----------+---------------+-----------+------+-------+--------+-----------+\n");
    for (size_t i = 0; i < len; i++) {
        if (!arr[i].disponible) continue;
        printf("|%3d | %-9s | %-13s | %-9s | %-4s | %4d | %-6s | %9.2f |\n",
               arr[i].id, arr[i].placa, arr[i].marca, arr[i].modelo, arr[i].tipo,
               arr[i].anio, arr[i].estado, arr[i].precio);
    }
    printf("+----+-----------+---------------+-----------+------+-------+--------+-----------+\n");
    free(arr);
}

void listarVehiculosDisponibles(void) {
    Vehiculo *arr = NULL; size_t len = 0; cargarVehiculos(&arr, &len);
    for (size_t i=0;i<len;i++) {
        if (arr[i].disponible) {
            printf("%zu. %s %s - $%.2f\n", i+1, arr[i].marca, arr[i].modelo, arr[i].precio);
        }
    }
    free(arr);
}

static void str_to_lower(const char *src, char *dst, size_t n) {
    if (!src || !dst) return;
    size_t i;
    for (i = 0; i < n-1 && src[i]; ++i) dst[i] = (char)tolower((unsigned char)src[i]);
    dst[i] = '\0';
}

/* Recomendar un vehiculo segun criterios: marca, anio minimo, estado (nuevo/usado), rango precio, modelo */
void recomendarVehiculo(void) {
    Vehiculo *arr = NULL; size_t len = 0; cargarVehiculos(&arr, &len);
    if (!arr || len == 0) { printf("No hay vehiculos en el sistema.\n"); free(arr); return; }

    char in_marca[64] = "", in_modelo[64] = "";
    int ano_min = 0;
    int estado_choice = 0; /* 0=ambos,1=nuevo,2=usado */
    double pmin = 0.0, pmax = 0.0;

    /* permitir entrada vacia para marca/modelo (cualquiera) */
    printf("Marca (vacio = cualquiera): "); if (!fgets(in_marca, sizeof(in_marca), stdin)) in_marca[0]='\0'; trim_newline(in_marca);
    printf("Modelo (vacio = cualquiera): "); if (!fgets(in_modelo, sizeof(in_modelo), stdin)) in_modelo[0]='\0'; trim_newline(in_modelo);
    ano_min = pedirEnteroLocal("Ano minimo (0 = cualquiera): ");
    estado_choice = pedirEnteroLocal("Estado (0=Ambos,1=Nuevo,2=Usado): ");
    pmin = pedirDoubleLocal("Precio minimo (0 = cualquiera): ");
    pmax = pedirDoubleLocal("Precio maximo (0 = cualquiera): ");
    if (pmax == 0.0) pmax = 1e12;

    /* preparar versiones en minuscula para comparaciones */
    char lm[64], lmod[64], low_field[64];
    str_to_lower(in_marca, lm, sizeof(lm));
    str_to_lower(in_modelo, lmod, sizeof(lmod));

    int found = 0;
    printf("\nRecomendaciones:\n");
    printf("+----+-----------+---------------+-----------+------+-------+--------+-----------+\n");
    printf("| ID | Placa     | Marca         | Modelo    | Tipo | Ano   | Estado | Precio    |\n");
    printf("+----+-----------+---------------+-----------+------+-------+--------+-----------+\n");
    for (size_t i = 0; i < len; ++i) {
        if (!arr[i].disponible) continue;
        /* marca */
        if (lm[0]) {
            str_to_lower(arr[i].marca, low_field, sizeof(low_field));
            if (!strstr(low_field, lm)) continue;
        }
        /* modelo */
        if (lmod[0]) {
            str_to_lower(arr[i].modelo, low_field, sizeof(low_field));
            if (!strstr(low_field, lmod)) continue;
        }
        /* ano */
        if (ano_min > 0 && arr[i].anio < ano_min) continue;
        /* estado */
        if (estado_choice == 1) { /* nuevo */
            str_to_lower(arr[i].estado, low_field, sizeof(low_field)); if (strstr(low_field, "nuevo") == NULL) continue;
        } else if (estado_choice == 2) { /* usado */
            str_to_lower(arr[i].estado, low_field, sizeof(low_field)); if (strstr(low_field, "usad") == NULL) continue;
        }
        /* precio */
        if (!(arr[i].precio >= pmin && arr[i].precio <= pmax)) continue;

        printf("|%3d | %-9s | %-13s | %-9s | %-4s | %4d | %-6s | %9.2f |\n",
               arr[i].id, arr[i].placa, arr[i].marca, arr[i].modelo, arr[i].tipo,
               arr[i].anio, arr[i].estado, arr[i].precio);
        found = 1;
    }
    printf("+----+-----------+---------------+-----------+------+-------+--------+-----------+\n");
    if (!found) printf("No se encontraron vehiculos que coincidan con los criterios.\n");
    free(arr);
}

void editarVehiculo(void) {
    Vehiculo *arr = NULL; size_t len = 0; cargarVehiculos(&arr, &len);
    if (len==0) { printf("No hay vehiculos.\n"); return; }
    listarVehiculos();
    int id = pedirEnteroLocal("Ingrese ID del vehiculo a editar: ");
    size_t idx = SIZE_MAX;
    for (size_t i=0;i<len;i++) if (arr[i].id==id) { idx=i; break; }
    if (idx==SIZE_MAX) { printf("ID no encontrado.\n"); free(arr); return; }
    int opt = pedirEnteroLocal("1.Placa 2.Marca 3.Modelo 4.Tipo 5.Ano 6.Estado 7.Precio 8.Todos\nElija: ");
    if (opt==1 || opt==8) { pedirCadenaLocal("Nueva placa: ", arr[idx].placa, sizeof(arr[idx].placa), 1, 0, 1); }
    if (opt==2 || opt==8) { pedirCadenaLocal("Nueva marca: ", arr[idx].marca, sizeof(arr[idx].marca), 0, 1, 0); }
    if (opt==3 || opt==8) { pedirCadenaLocal("Nuevo modelo: ", arr[idx].modelo, sizeof(arr[idx].modelo), 0, 1, 0); }
    if (opt==4 || opt==8) { pedirCadenaLocal("Nuevo tipo: ", arr[idx].tipo, sizeof(arr[idx].tipo), 0, 1, 0); }
    if (opt==5 || opt==8) { arr[idx].anio = pedirEnteroLocal("Nuevo ano: "); }
    if (opt==6 || opt==8) { pedirCadenaLocal("Nuevo estado: ", arr[idx].estado, sizeof(arr[idx].estado), 0, 0, 0); }
    if (opt==7 || opt==8) { arr[idx].precio = pedirDoubleLocal("Nuevo precio: "); }
    guardarVehiculos(arr, len);
    free(arr);
    printf("Vehiculo actualizado.\n");
}

void eliminarVehiculo(void) {
    Vehiculo *arr = NULL; size_t len = 0; cargarVehiculos(&arr, &len);
    if (len==0) { printf("No hay vehiculos.\n"); return; }
    listarVehiculos(); int id = pedirEnteroLocal("Ingrese ID del vehiculo a eliminar: ");
    size_t idx = SIZE_MAX; for (size_t i=0;i<len;i++) if (arr[i].id==id) { idx=i; break; }
    if (idx==SIZE_MAX) { printf("ID no encontrado.\n"); free(arr); return; }
    for (size_t i = idx; i + 1 < len; i++) {
        arr[i] = arr[i+1];
    }
    len--;
    guardarVehiculos(arr, len);
    free(arr);
    printf("Vehiculo eliminado.\n");
}

/* ---------- Clientes (archivo: clientes.dat) ---------- */
void cargarClientes(Cliente **arr, size_t *len) {
    *arr = NULL; *len = 0; FILE *f = fopen("clientes.dat","r"); if(!f) return;
    char linea[512]; size_t cap=0; while (fgets(linea,sizeof(linea),f)){
        trim_newline(linea); if(linea[0]=='\0') continue; char *tok = strtok(linea,"|,"); if(!tok) continue;
        Cliente c={0}; c.id=atoi(tok);
        tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(c.cedula,tok,sizeof(c.cedula)-1);
        tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(c.nombre,tok,sizeof(c.nombre)-1);
        tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(c.telefono,tok,sizeof(c.telefono)-1);
        tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(c.correo,tok,sizeof(c.correo)-1);
        tok=strtok(NULL,"\n"); if(tok) strncpy(c.direccion,tok,sizeof(c.direccion)-1);
        if(*len+1>cap){ size_t ncap=cap==0?16:cap*2; Cliente *tmp=realloc(*arr,ncap*sizeof(Cliente)); if(!tmp){free(*arr);*arr=NULL;*len=0;fclose(f);return;} *arr=tmp; cap=ncap; }
        (*arr)[(*len)++]=c;
    }
    fclose(f);
}

void guardarClientes(Cliente *arr, size_t len) {
    FILE *f = fopen("clientes.dat","w");
    if (!f) return;
    for (size_t i = 0; i < len; i++) {
        fprintf(f, "%d|%s|%s|%s|%s|%s\n", arr[i].id, arr[i].cedula, arr[i].nombre, arr[i].telefono, arr[i].correo, arr[i].direccion);
    }
    fclose(f);
}

void agregarCliente(void) {
    Cliente *arr = NULL; size_t len = 0; cargarClientes(&arr, &len);
    Cliente c = {0}; c.id = (len==0) ? 1 : arr[len-1].id+1;
    pedirCadenaLocal("Cedula: ", c.cedula, sizeof(c.cedula), 1, 0, 0);
    pedirCadenaLocal("Nombre: ", c.nombre, sizeof(c.nombre), 0, 1, 0);
    pedirCadenaLocal("Telefono: ", c.telefono, sizeof(c.telefono), 1, 0, 0);
    pedirCadenaLocal("Correo: ", c.correo, sizeof(c.correo), 1, 0, 1);
    pedirCadenaLocal("Direccion: ", c.direccion, sizeof(c.direccion), 1, 1, 1);
    Cliente *tmp = realloc(arr, (len+1)*sizeof(Cliente));
    if (!tmp) { free(arr); printf("Error memoria\n"); return; }
    arr = tmp; arr[len++] = c; guardarClientes(arr, len); free(arr); printf("Cliente agregado.\n");
}

void listarClientes(void){ 
    Cliente *arr = NULL; size_t len = 0; cargarClientes(&arr, &len);
    for (size_t i = 0; i < len; i++) printf("%d. %s\n", arr[i].id, arr[i].nombre);
    free(arr);
}

void editarCliente(void) {
    Cliente *arr = NULL; size_t len = 0; cargarClientes(&arr, &len);
    if (len==0){ printf("No hay clientes.\n"); return; }
    listarClientes(); int id = pedirEnteroLocal("Ingrese ID del cliente a editar: ");
    size_t idx = SIZE_MAX; for (size_t i=0;i<len;i++) if (arr[i].id==id) { idx=i; break; }
    if (idx==SIZE_MAX){ printf("ID no encontrado.\n"); free(arr); return; }
    /* Guardar nombre previo para actualizar ventas.dat */
    char prevName[128]; strncpy(prevName, arr[idx].nombre, sizeof(prevName)-1); prevName[sizeof(prevName)-1]='\0';
    int opt = pedirEnteroLocal("1.Cedula 2.Nombre 3.Telefono 4.Correo 5.Direccion 6.Todos\nElija: ");
    if (opt==1||opt==6){ pedirCadenaLocal("Nueva cedula: ", arr[idx].cedula, sizeof(arr[idx].cedula), 1, 0, 0); }
    if (opt==2||opt==6){ pedirCadenaLocal("Nuevo nombre: ", arr[idx].nombre, sizeof(arr[idx].nombre), 0, 1, 0); }
    if (opt==3||opt==6){ pedirCadenaLocal("Nuevo telefono: ", arr[idx].telefono, sizeof(arr[idx].telefono), 1, 0, 0); }
    if (opt==4||opt==6){ pedirCadenaLocal("Nuevo correo: ", arr[idx].correo, sizeof(arr[idx].correo), 1, 0, 1); }
    if (opt==5||opt==6){ pedirCadenaLocal("Nueva direccion: ", arr[idx].direccion, sizeof(arr[idx].direccion), 1, 1, 1); }
    guardarClientes(arr, len);
    /* Actualizar ventas.dat: reemplazar el nombre antiguo por el nuevo en los registros de venta */
    FILE *f = fopen("ventas.dat","r");
    if (f) {
        VentaExt *varr = NULL; size_t vcap=0, vlen=0;
        char line[512];
        while (fgets(line, sizeof(line), f)) {
            trim_newline(line); if(line[0]=='\0') continue; char *s=line; char *tok=strtok(s,"|,"); if(!tok) continue; VentaExt v={0}; v.id=atoi(tok);
            tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.fecha,tok,sizeof(v.fecha)-1);
            tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.cliente,tok,sizeof(v.cliente)-1);
            tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.vendedor,tok,sizeof(v.vendedor)-1);
            tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.vehiculo,tok,sizeof(v.vehiculo)-1);
            tok=strtok(NULL,"\n"); v.precio = tok?atof(tok):0.0;
            if (vlen+1>vcap){ size_t ncap=vcap==0?16:vcap*2; VentaExt *tmp=realloc(varr,ncap*sizeof(VentaExt)); if(!tmp){ free(varr); varr=NULL; vlen=0; break; } varr=tmp; vcap=ncap; }
            varr[vlen++]=v;
        }
        fclose(f);
        if (vlen>0 && varr) {
            int changed=0;
            for (size_t i=0;i<vlen;i++) if (strcmp(varr[i].cliente, prevName)==0) { strncpy(varr[i].cliente, arr[idx].nombre, sizeof(varr[i].cliente)-1); varr[i].cliente[sizeof(varr[i].cliente)-1]='\0'; changed=1; }
            if (changed) {
                FILE *fw = fopen("ventas.dat","w"); if (fw) { for (size_t i=0;i<vlen;i++) fprintf(fw,"%d|%s|%s|%s|%s|%.2f\n", varr[i].id, varr[i].fecha, varr[i].cliente, varr[i].vendedor, varr[i].vehiculo, varr[i].precio); fclose(fw); }
            }
        }
        free(varr);
    }
    free(arr);
    printf("Cliente actualizado.\n");
}

void eliminarCliente(void) {
    Cliente *arr = NULL; size_t len = 0; cargarClientes(&arr, &len);
    if (len==0) { printf("No hay clientes.\n"); return; }
    listarClientes(); int id = pedirEnteroLocal("Ingrese ID del cliente a eliminar: ");
    size_t idx = SIZE_MAX; for (size_t i=0;i<len;i++) if (arr[i].id==id) { idx=i; break; }
    if (idx==SIZE_MAX) { printf("ID no encontrado.\n"); free(arr); return; }
    for (size_t i = idx; i + 1 < len; i++) arr[i] = arr[i+1];
    len--;
    guardarClientes(arr, len);
    free(arr);
    printf("Cliente eliminado.\n");
}

/* ---------- Vendedores (archivo: vendedores.dat) ---------- */
void cargarVendedores(Vendedor **arr, size_t *len){ *arr=NULL; *len=0; FILE *f=fopen("vendedores.dat","r"); if(!f) return; char linea[512]; size_t cap=0; while(fgets(linea,sizeof(linea),f)){ trim_newline(linea); if(linea[0]=='\0') continue; char *tok=strtok(linea,"|,"); if(!tok) continue; Vendedor v={0}; v.id=atoi(tok);
    tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.codigo,tok,sizeof(v.codigo)-1);
    tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.nombre,tok,sizeof(v.nombre)-1);
    tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.telefono,tok,sizeof(v.telefono)-1);
    tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.correo,tok,sizeof(v.correo)-1);
    /* campo opcional: comision acumulada */
    tok = strtok(NULL, "|,"); if (tok) v.comision = atof(tok); else v.comision = 0.0;
    if(*len+1>cap){ size_t ncap=cap==0?16:cap*2; Vendedor *tmp=realloc(*arr,ncap*sizeof(Vendedor)); if(!tmp){free(*arr);*arr=NULL;*len=0;fclose(f);return;} *arr=tmp; cap=ncap;} (*arr)[(*len)++]=v; } fclose(f); }

void guardarVendedores(Vendedor *arr, size_t len){ FILE *f=fopen("vendedores.dat","w"); if(!f) return; for(size_t i=0;i<len;i++) fprintf(f,"%d|%s|%s|%s|%s|%.2f\n", arr[i].id, arr[i].codigo, arr[i].nombre, arr[i].telefono, arr[i].correo, arr[i].comision); fclose(f); }

void agregarVendedor(void) {
    Vendedor *arr = NULL; size_t len = 0; cargarVendedores(&arr, &len);
    Vendedor v = {0}; v.id = (len==0) ? 1 : arr[len-1].id+1;
    pedirCadenaLocal("Codigo: ", v.codigo, sizeof(v.codigo), 1, 0, 0);
    pedirCadenaLocal("Nombre: ", v.nombre, sizeof(v.nombre), 0, 1, 0);
    pedirCadenaLocal("Telefono: ", v.telefono, sizeof(v.telefono), 1, 0, 0);
    pedirCadenaLocal("Correo: ", v.correo, sizeof(v.correo), 1, 0, 1);
    v.comision = 0.0;
    Vendedor *tmp = realloc(arr, (len+1)*sizeof(Vendedor)); if(!tmp){ free(arr); printf("Error memoria\n"); return; }
    arr = tmp; arr[len++] = v; guardarVendedores(arr, len); free(arr); printf("Vendedor agregado.\n");
}

void listarVendedores(void){ Vendedor *arr=NULL; size_t len=0; cargarVendedores(&arr,&len); for(size_t i=0;i<len;i++) printf("%d. %s\n", arr[i].id, arr[i].nombre); free(arr); }

void editarVendedor(void) {
    Vendedor *arr = NULL; size_t len = 0; cargarVendedores(&arr, &len);
    if (len==0) { printf("No hay vendedores.\n"); return; }
    listarVendedores(); int id = pedirEnteroLocal("Ingrese ID del vendedor a editar: ");
    size_t idx = SIZE_MAX; for (size_t i=0;i<len;i++) if (arr[i].id==id) { idx=i; break; }
    if (idx==SIZE_MAX) { printf("ID no encontrado.\n"); free(arr); return; }
    /* Guardar nombre previo para actualizar ventas.dat */
    char prevName[128]; strncpy(prevName, arr[idx].nombre, sizeof(prevName)-1); prevName[sizeof(prevName)-1] = '\0';
    int opt = pedirEnteroLocal("1.Codigo 2.Nombre 3.Telefono 4.Correo 5.Todos\nElija: ");
    if (opt==1||opt==5){ pedirCadenaLocal("Nuevo codigo: ", arr[idx].codigo, sizeof(arr[idx].codigo), 1, 0, 0); }
    if (opt==2||opt==5){ pedirCadenaLocal("Nuevo nombre: ", arr[idx].nombre, sizeof(arr[idx].nombre), 0, 1, 0); }
    if (opt==3||opt==5){ pedirCadenaLocal("Nuevo telefono: ", arr[idx].telefono, sizeof(arr[idx].telefono), 1, 0, 0); }
    if (opt==4||opt==5){ pedirCadenaLocal("Nuevo correo: ", arr[idx].correo, sizeof(arr[idx].correo), 1, 0, 1); }
    guardarVendedores(arr, len);
    /* Actualizar ventas.dat reemplazando el nombre antiguo por el nuevo */
    FILE *f = fopen("ventas.dat","r");
    if (f) {
        VentaExt *varr = NULL; size_t vcap=0, vlen=0; char line[512];
        while (fgets(line, sizeof(line), f)) {
            trim_newline(line); if(line[0]=='\0') continue; char *s=line; char *tok=strtok(s,"|,"); if(!tok) continue; VentaExt v={0}; v.id=atoi(tok);
            tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.fecha,tok,sizeof(v.fecha)-1);
            tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.cliente,tok,sizeof(v.cliente)-1);
            tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.vendedor,tok,sizeof(v.vendedor)-1);
            tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.vehiculo,tok,sizeof(v.vehiculo)-1);
            tok=strtok(NULL,"\n"); v.precio = tok?atof(tok):0.0;
            if (vlen+1>vcap){ size_t ncap=vcap==0?16:vcap*2; VentaExt *tmp=realloc(varr,ncap*sizeof(VentaExt)); if(!tmp){ free(varr); varr=NULL; vlen=0; break; } varr=tmp; vcap=ncap; }
            varr[vlen++]=v;
        }
        fclose(f);
        if (vlen>0 && varr) {
            int changed=0;
            for (size_t i=0;i<vlen;i++) if (strcmp(varr[i].vendedor, prevName)==0) { strncpy(varr[i].vendedor, arr[idx].nombre, sizeof(varr[i].vendedor)-1); varr[i].vendedor[sizeof(varr[i].vendedor)-1]='\0'; changed=1; }
            if (changed) {
                FILE *fw = fopen("ventas.dat","w"); if (fw) { for (size_t i=0;i<vlen;i++) fprintf(fw,"%d|%s|%s|%s|%s|%.2f\n", varr[i].id, varr[i].fecha, varr[i].cliente, varr[i].vendedor, varr[i].vehiculo, varr[i].precio); fclose(fw); }
            }
        }
        free(varr);
    }
    free(arr);
    printf("Vendedor actualizado.\n");
}

void eliminarVendedor(void){ Vendedor *arr=NULL; size_t len=0; cargarVendedores(&arr,&len); if(len==0){printf("No hay vendedores.\n");return;} listarVendedores(); int id = pedirEnteroLocal("Ingrese ID del vendedor a eliminar: "); size_t idx=SIZE_MAX; for(size_t i=0;i<len;i++) if(arr[i].id==id){idx=i;break;} if(idx==SIZE_MAX){printf("ID no encontrado.\n");free(arr);return;} for(size_t i=idx;i+1<len;i++) arr[i]=arr[i+1]; len--; guardarVendedores(arr,len); free(arr); printf("Vendedor eliminado.\n"); }

/* ---------- Ventas (archivo: ventas.dat) ---------- */
static int compararVentaFechaDesc(const void *a, const void *b){ const VentaExt *va=(const VentaExt*)a; const VentaExt *vb=(const VentaExt*)b; return strcmp(vb->fecha, va->fecha); }

void registrarVenta(void){
    Vehiculo *varr=NULL; size_t vlen=0; cargarVehiculos(&varr,&vlen);
    if(vlen==0){ printf("No hay vehiculos.\n"); return; }
    printf("=== VEHICULOS DISPONIBLES ===\n");
    size_t *indices = malloc(vlen * sizeof(size_t)); size_t available = 0;
    for(size_t i=0;i<vlen;i++) if(varr[i].disponible){ indices[available++]=i; printf("%zu. %s %s - $%.2f\n", available, varr[i].marca, varr[i].modelo, varr[i].precio); }
    if(available==0){ printf("No hay vehiculos disponibles.\n"); free(indices); free(varr); return; }
    int sel = pedirEnteroLocal("Seleccione el vehiculo: "); if(sel<1 || (size_t)sel>available){ printf("Seleccion invalida.\n"); free(indices); free(varr); return; }
    size_t vidx = indices[sel-1]; printf("Vehiculo seleccionado:\n%s %s\n", varr[vidx].marca, varr[vidx].modelo);

    Cliente *carr=NULL; size_t clen=0; cargarClientes(&carr,&clen); if(clen==0){ printf("No hay clientes. Registre uno primero.\n"); free(indices); free(varr); return; }
    printf("=== CLIENTES REGISTRADOS ===\n"); for(size_t i=0;i<clen;i++) printf("%zu. %s\n", i+1, carr[i].nombre);
    int csel = pedirEnteroLocal("Seleccione el cliente: "); if(csel<1 || (size_t)csel>clen){ printf("Seleccion invalida.\n"); free(indices); free(varr); free(carr); return; }
    size_t cidx = csel-1; printf("Cliente seleccionado:\n%s\n", carr[cidx].nombre);

    Vendedor *varr2=NULL; size_t v2len=0; cargarVendedores(&varr2,&v2len); if(v2len==0){ printf("No hay vendedores. Registre uno primero.\n"); free(indices); free(varr); free(carr); return; }
    printf("=== VENDEDORES REGISTRADOS ===\n"); for(size_t i=0;i<v2len;i++) printf("%zu. %s\n", i+1, varr2[i].nombre);
    int vsel = pedirEnteroLocal("Seleccione el vendedor: "); if(vsel<1 || (size_t)vsel>v2len){ printf("Seleccion invalida.\n"); free(indices); free(varr); free(carr); free(varr2); return; }
    size_t vendx = vsel-1; printf("Vendedor seleccionado:\n%s\n", varr2[vendx].nombre);

    printf("\nResumen:\nCliente: %s\nVendedor: %s\nVehiculo: %s %s\nPrecio: $%.2f\n", carr[cidx].nombre, varr2[vendx].nombre, varr[vidx].marca, varr[vidx].modelo, varr[vidx].precio);
    int conf = pedirEnteroLocal("Confirmar venta? 1.Si 2.No : "); if(conf!=1){ printf("Venta cancelada.\n"); free(indices); free(varr); free(carr); free(varr2); return; }

    /* Eliminar vehículo de vehiculos.dat (no mantener en inventario) */
    for (size_t i = vidx; i + 1 < vlen; i++) varr[i] = varr[i+1];
    vlen--;
    guardarVehiculos(varr, vlen);

    /* Registrar venta en ventas.dat */
    FILE *f = fopen("ventas.dat","a");
    if(!f){ printf("No se pudo abrir ventas.dat para escritura.\n"); free(indices); free(varr); free(carr); free(varr2); return; }
    /* id = timestamp simple */
    time_t t = time(NULL); struct tm tm; localtime_s(&tm, &t); char fecha[11]; strftime(fecha, sizeof(fecha), "%Y-%m-%d", &tm);
    /* generar id incremental leyendo archivo */
    int nextid = 1; FILE *fr = fopen("ventas.dat","r"); if(fr){ char buf[512]; int last=0; while(fgets(buf,sizeof(buf),fr)){ char *tk=strtok(buf,"|,"); if(tk) last=atoi(tk); } fclose(fr); nextid = last+1; }
    char vehdesc[96]; snprintf(vehdesc, sizeof(vehdesc), "%s %s", varr[vidx].marca, varr[vidx].modelo);
    fprintf(f, "%d|%s|%s|%s|%s|%.2f\n", nextid, fecha, carr[cidx].nombre, varr2[vendx].nombre, vehdesc, varr[vidx].precio);
    fclose(f);
    /* Calcular y asignar comision al vendedor */
    double commission = varr[vidx].precio * COMMISSION_RATE;
    varr2[vendx].comision += commission;
    guardarVendedores(varr2, v2len);
    printf("Comision %.2f agregada a vendedor %s\n", commission, varr2[vendx].nombre);
    printf("Venta registrada y archivo actualizado.\n");
    free(indices); free(varr); free(carr); free(varr2);
}

/* Listar vehiculos vendidos (leer ventas.dat) */
void listarVehiculosVendidos(void) {
    const char *ruta = "ventas.dat";
    FILE *f = fopen(ruta, "r");
    if (!f) { printf("No hay ventas registradas.\n"); return; }
    char linea[512]; int idx = 1;
    printf("--- Vehiculos vendidos ---\n");
    while (fgets(linea, sizeof(linea), f)) {
        trim_newline(linea);
        if (linea[0] == '\0') continue;
        char *tok = strtok(linea, "|,"); if (!tok) continue; int id = atoi(tok);
        tok = strtok(NULL, "|,"); if (!tok) continue; char fecha[16]; strncpy(fecha, tok, sizeof(fecha)-1);
        tok = strtok(NULL, "|,"); if (!tok) continue; char cliente[64]; strncpy(cliente, tok, sizeof(cliente)-1);
        tok = strtok(NULL, "|,"); if (!tok) continue; char vendedor[64]; strncpy(vendedor, tok, sizeof(vendedor)-1);
        tok = strtok(NULL, "|,"); if (!tok) continue; char vehiculo[96]; strncpy(vehiculo, tok, sizeof(vehiculo)-1);
        tok = strtok(NULL, "\n"); double precio = tok ? atof(tok) : 0.0;
        printf("%d) ID:%d | %s | %s | %s | %s | $%.2f\n", idx++, id, fecha, cliente, vendedor, vehiculo, precio);
    }
    fclose(f);
}

/* Editar información del vehículo dentro de un registro de venta */
void editarVehiculoVendido(void) {
    const char *ruta = "ventas.dat";
    FILE *f = fopen(ruta, "r");
    if (!f) { printf("No hay ventas registradas.\n"); return; }
    VentaExt *arr = NULL; size_t cap = 0, len = 0;
    char linea[512];
    while (fgets(linea, sizeof(linea), f)) {
        trim_newline(linea);
        if (linea[0] == '\0') continue;
        char *s = linea; char *tok = strtok(s, "|,"); if (!tok) continue; VentaExt v = {0}; v.id = atoi(tok);
        tok = strtok(NULL, "|,"); if (!tok) continue; strncpy(v.fecha, tok, sizeof(v.fecha)-1);
        tok = strtok(NULL, "|,"); if (!tok) continue; strncpy(v.cliente, tok, sizeof(v.cliente)-1);
        tok = strtok(NULL, "|,"); if (!tok) continue; strncpy(v.vendedor, tok, sizeof(v.vendedor)-1);
        tok = strtok(NULL, "|,"); if (!tok) continue; strncpy(v.vehiculo, tok, sizeof(v.vehiculo)-1);
        tok = strtok(NULL, "\n"); v.precio = tok ? atof(tok) : 0.0;
        if (len + 1 > cap) { size_t ncap = cap==0?16:cap*2; VentaExt *tmp = realloc(arr, ncap*sizeof(VentaExt)); if(!tmp){ free(arr); fclose(f); printf("Error memoria\n"); return; } arr = tmp; cap = ncap; }
        arr[len++] = v;
    }
    fclose(f);
    if (len == 0) { printf("No hay ventas registradas.\n"); free(arr); return; }

    printf("Seleccione venta a editar:\n");
    for (size_t i=0;i<len;i++) printf("%zu) ID:%d | %s | %s | %s | %s | $%.2f\n", i+1, arr[i].id, arr[i].fecha, arr[i].cliente, arr[i].vendedor, arr[i].vehiculo, arr[i].precio);
    int sel = pedirEnteroLocal("Ingrese numero: ");
    if (sel < 1 || (size_t)sel > len) { printf("Seleccion invalida.\n"); free(arr); return; }
    size_t idx = sel-1;
    int opt = pedirEnteroLocal("Editar: 1) Vehiculo 2) Precio 3) Cliente 4) Vendedor 5) Todos 0) Cancelar\nElija: ");
    char buffer[128];
    if (opt == 1 || opt == 5) { pedirCadenaLocal("Nuevo valor Vehiculo: ", buffer, sizeof(buffer), 0, 1, 1); strncpy(arr[idx].vehiculo, buffer, sizeof(arr[idx].vehiculo)-1); }
    if (opt == 2 || opt == 5) { arr[idx].precio = pedirDoubleLocal("Nuevo Precio: "); }
    if (opt == 3 || opt == 5) { pedirCadenaLocal("Nuevo Cliente: ", buffer, sizeof(buffer), 0, 1, 1); strncpy(arr[idx].cliente, buffer, sizeof(arr[idx].cliente)-1); }
    if (opt == 4 || opt == 5) { pedirCadenaLocal("Nuevo Vendedor: ", buffer, sizeof(buffer), 0, 1, 1); strncpy(arr[idx].vendedor, buffer, sizeof(arr[idx].vendedor)-1); }

    /* Guardar ventas.dat */
    FILE *fw = fopen(ruta, "w"); if (!fw) { printf("No se pudo actualizar ventas.dat\n"); free(arr); return; }
    for (size_t i=0;i<len;i++) fprintf(fw, "%d|%s|%s|%s|%s|%.2f\n", arr[i].id, arr[i].fecha, arr[i].cliente, arr[i].vendedor, arr[i].vehiculo, arr[i].precio);
    fclose(fw); free(arr); printf("Registro de venta actualizado.\n");
}


void mostrarVentas(void){
    const char *ruta = "ventas.dat";
    FILE *f = fopen(ruta, "r");
    if (!f) { printf("No se encontro el archivo de ventas ('%s').\n", ruta); return; }
    VentaExt *arr = NULL; size_t cap=0,len=0; char linea[512];
    while(fgets(linea,sizeof(linea),f)){
        trim_newline(linea); if(linea[0]=='\0') continue; char *tok=strtok(linea,"|,"); if(!tok) continue; VentaExt v={0}; v.id=atoi(tok);
        tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.fecha,tok,sizeof(v.fecha)-1);
        tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.cliente,tok,sizeof(v.cliente)-1);
        tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.vendedor,tok,sizeof(v.vendedor)-1);
        tok=strtok(NULL,"|,"); if(!tok) continue; strncpy(v.vehiculo,tok,sizeof(v.vehiculo)-1);
        tok=strtok(NULL,"\n"); if(!tok) continue; v.precio=atof(tok);
        if(len+1>cap){ size_t ncap=cap==0?16:cap*2; VentaExt *tmp=realloc(arr,ncap*sizeof(VentaExt)); if(!tmp){free(arr);fclose(f);printf("Error memoria\n");return;} arr=tmp; cap=ncap; }
        arr[len++]=v;
    }
    fclose(f);
    if(len==0){ printf("No hay ventas registradas.\n"); free(arr); return; }
    qsort(arr,len,sizeof(VentaExt),compararVentaFechaDesc);
    printf("+----+------------+----------------------+----------------------+-------------------------------+-----------+\n");
    printf("| ID | Fecha      | Cliente              | Vendedor             | Vehiculo                      | Precio    |\n");
    printf("+----+------------+----------------------+----------------------+-------------------------------+-----------+\n");
    for(size_t i=0;i<len;i++) printf("| %2d | %10s | %-20s | %-20s | %-29s | %9.2f |\n", arr[i].id, arr[i].fecha, arr[i].cliente, arr[i].vendedor, arr[i].vehiculo, arr[i].precio);
    printf("+----+------------+----------------------+----------------------+-------------------------------+-----------+\n");
    free(arr);
}


