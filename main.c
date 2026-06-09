#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

int main(int argc, char *argv[]) {
    // Si no se proveen los parametros por consola, definimos uno por defecto para no frenar la ejecucion
    char* archivoBinario = ARCH_ENUT_MOD;
    if(argc == 2) {
        archivoBinario = argv[1];
    }

    int opcion;

    do {
        printf("\n========================================================\n");
        printf("  TRABAJO PRACTICO ENUT 2021 - TOPICOS DE PROGRAMACION\n");
        printf("========================================================\n");
        printf(" 1. Probar Puntos 1 y 2 (Lucas)\n");
        printf(" 2. Probar Puntos 3 y 4 (Tomas)\n");
        printf(" 3. Probar Puntos 5, 6 y 7 (Fer)\n");
        printf(" 4. Probar Punto 8 (Elias)\n");
        printf(" 5. Probar Puntos 9, 10 y 11 (Matias)\n");
        printf(" 0. Salir\n");
        printf("========================================================\n");
        printf("Ingrese una opcion: ");

        // Validacion simple para evitar un loop infinito si el usuario ingresa un caracter en lugar de un numero
        if (scanf("%d", &opcion) != 1) {
            while (getchar() != '\n'); // Limpia el buffer
            opcion = -1;
        }

        switch(opcion) {
            case 1:
                printf("\n--- EJECUTANDO PUNTOS 1 Y 2 ---\n");
                resolucionPunto1();
                resolucionPunto2();
                break;

            case 2:
                printf("\n--- EJECUTANDO PUNTOS 3 Y 4 ---\n");
                Puntos3y4(ARCH_ENUT);
                break;

            case 3:
                printf("\n--- EJECUTANDO PUNTOS 5, 6 Y 7 ---\n");
                resolucionPunto5();
                resolucionPunto6();
                resolucionPunto7();
                break;

            case 4:
                printf("\n--- EJECUTANDO PUNTO 8 ---\n");
                punto_8(ARCH_ENUT, ARCH_ENUT_MOD);
                BinATexto(ARCH_ENUT_MOD, "enut2021_procesado_prueba.csv");
                printf(">> Punto 8: Archivo binario '%s' y CSV de prueba generados con exito.\n", ARCH_ENUT_MOD);
                // leerBinario(ARCH_ENUT_MOD); // Opcional: Descomentar si queres ver el binario por consola
                break;

            case 5:
                printf("\n--- EJECUTANDO PUNTOS 9, 10 Y 11 ---\n");
                // Las llaves aca son necesarias para poder declarar variables dentro de un 'case' en C
                {
                    Vector vec;
                    if (!CrearVector(&vec, sizeof(tRegModif), 10000)) {
                        printf("Error: No se pudo asignar memoria para el vector.\n");
                        break;
                    }

                    if (bajartxt(archivoBinario, &vec) != 0) {
                        printf("Error: No se pudo leer el archivo binario '%s'. Recorda ejecutar el Punto 8 primero.\n", archivoBinario);
                        DestruirVector(&vec);
                        break;
                    }

                    printf("\n>>> Punto 9");
                    OrdenarSeleccion(&vec, cmpTipoTrabajo);
                    Punto_9(&vec);

                    printf("\n>>> Punto 10");
                    Punto_10(&vec);

                    printf("\n>>> Punto 11");
                    OrdenarSeleccion(&vec, cmpNivelSexoTipo);
                    Punto_11(&vec);

                    DestruirVector(&vec);
                }
                break;

            case 0:
                printf("\nSaliendo del programa. ¡Exitos en la entrega!\n");
                break;

            default:
                printf("\nOpcion no valida. Por favor, ingrese un numero del 0 al 5.\n");
        }

    } while(opcion != 0);

    return 0;
}
