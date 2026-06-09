#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LINEA 65536
#define CANT_REGIONES 6
#define CANT_CAMPOS 14
#define CANT_TRABAJOS 3

#define ARCH_ENUT "enut2021_base.csv"
#define ARCH_ENUT_MOD "enut2021_procesado.bin"

// --- ESTRUCTURAS PUNTOS 1 Y 2 ---
typedef struct {
    int registros;
    double hogaresEstimados;
    double personasEstimadas;
} ResumenRegion;

// --- ESTRUCTURAS PUNTOS 5, 6 Y 7 ---
typedef struct {
    double hogaresNinguno;
    double hogaresExclusivo;
} RegionCuidado;

typedef struct {
    double totalPoblacion;
    double realizanActividad;
} Trabajo;

typedef struct {
    Trabajo trabajos[CANT_TRABAJOS];
} FilaReporte;

// --- ESTRUCTURAS PUNTO 8 ---
typedef struct {
    int id;
    int whog;
    int wper;
    int region;
    int sexo;
    int nivelEducativo;
    int tcs[4];
    int tp[4];
} tRegistroOrig;

typedef struct {
    int id;
    int whog;
    int wper;
    int region;
    int sexo_sel;
    int nivel_educativo;
    char tipo_trabajo[50];
    int tiempo;
    int valor;
} tRegModif;

// --- ESTRUCTURAS PUNTOS 9, 10 Y 11 ---
typedef struct {
    void* vec;
    int ce;
    size_t cap;
    size_t tamElem;
} Vector;

typedef int (*Cmp)(const void* a, const void* b);


// --- PROTOTIPOS: UTILIDADES GENERALES ---
int obtenerColumnas(char *header, const char *nombresColumnas[], int indicesColumnas[], int cantidadColumnas);
void quitarComillas(char *texto);
int esFaltante(const char *texto);
int convertirAEntero(const char *texto, int *valor);
int convertirADouble(const char *texto, double *valor);

const char *nombreRegion(int region);
const char *nombreSexo(int sexo);
const char *nombreGrupoEdad(int edad);

// --- PROTOTIPOS: PUNTOS DEL TP ---
void resolucionPunto1(void);
void resolucionPunto2(void);
void Puntos3y4(const char* ruta_archivo);
void resolucionPunto5(void);
void resolucionPunto6(void);
void resolucionPunto7(void);
void punto_8(const char *nombreArchOrigen, const char *nombreArchDest);
void leerBinario(const char *nombreArch);
void BinATexto(const char *nombreArchBin, const char *nombreArchTxt);

// Puntos 9, 10, 11
bool CrearVector(Vector* v, size_t tamElem, size_t cap);
bool redimensionar(Vector* v, size_t NuevaCap);
int InsertarAlFinal(Vector* v, void* elem);
void DestruirVector(Vector* v);
int bajartxt(const char* nomarch, Vector* vec);
void calculos(void* aux, long long* ponderado, long long* wper);
int Punto_9(Vector* v);
void OrdenarSeleccion(Vector *v, Cmp cmp);
int cmpTipoTrabajo(const void* v1, const void* v2);
int cmpNivelSexoTipo(const void* a, const void* b);
int Punto_10(Vector* v);
int Punto_11(Vector* v);
void** crear_matriz(size_t cant_filas, size_t cant_columnas, size_t tamElem);
void destruir_matriz(void** mat, size_t filas);
void InicializarCero(int **mat, size_t filas, size_t columnas);

#endif // FUNCIONES_H_INCLUDED
