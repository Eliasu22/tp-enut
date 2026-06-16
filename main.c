#include "funciones.h"

int main(int argc, char *argv[])
{

    char* archivoBinario = ARCH_ENUT_MOD;
    if(argc == 2)
    {
        archivoBinario = argv[1];
    }

    int opcion;

    do
    {
        // Limpia la pantalla antes de volver a dibujar el menú principal
        system("cls");

        printf("\n========================================================\n");
        printf("  TRABAJO PRACTICO ENUT 2021 - TOPICOS DE PROGRAMACION\n");
        printf("========================================================\n");
        printf(" 1. Probar Punto 1\n");
        printf(" 2. Probar Punto 2\n");
        printf(" 3. Probar Punto 3\n");
        printf(" 4. Probar Punto 4\n");
        printf(" 5. Probar Punto 5\n");
        printf(" 6. Probar Punto 6\n");
        printf(" 7. Probar Punto 7\n");
        printf(" 8. Probar Punto 8\n");
        printf(" 9. Probar Punto 9\n");
        printf(" 10. Probar Punto 10\n");
        printf(" 11. Probar Punto 11\n");
        printf(" 0. Salir\n");
        printf("========================================================\n");
        printf("Ingrese una opcion: ");

        // limpia el buffer
        if (scanf("%d", &opcion) != 1)
        {
            while (getchar() != '\n');
            opcion = -1;
        }

        if (opcion > 0 && opcion <= 11)
        {
            system("cls");
        }

        switch(opcion)
        {
        case 1:
            resolucionPunto1();
            break;
        case 2:
            resolucionPunto2();
            break;
        case 3:
            Punto3(ARCH_ENUT);
            break;
        case 4:
            Punto4(ARCH_ENUT);
            break;
        case 5:
            resolucionPunto5();
            break;
        case 6:
            resolucionPunto6();
            break;
        case 7:
            resolucionPunto7();
            break;
        case 8:
            punto_8(ARCH_ENUT, ARCH_ENUT_MOD);
            BinATexto(ARCH_ENUT_MOD, "enut2021_procesado_prueba.csv");
            printf("Punto 8: Archivo binario '%s' generado con exito.\n", ARCH_ENUT_MOD);
            leerBinario(ARCH_ENUT_MOD);
            break;
        case 9:
            printf("\nEjecutando Punto 9...\n");
            {
                Vector vec;
                if (CrearVector(&vec, sizeof(tRegModif), 10000) && bajartxt(archivoBinario, &vec) == 0)
                {
                    OrdenarSeleccion(&vec, cmpTipoTrabajo);
                    Punto_9(&vec);
                    DestruirVector(&vec);
                }
                else
                {
                    printf("Error: No se pudo leer el binario.  Ejecutar el Punto 8 primero.\n");
                }
            }
            break;
        case 10:
            printf("\nEjecutando Punto 10...\n");
            {
                Vector vec;
                if (CrearVector(&vec, sizeof(tRegModif), 10000) && bajartxt(archivoBinario, &vec) == 0)
                {
                    OrdenarSeleccion(&vec, cmpTipoTrabajo);
                    Punto_10(&vec);
                    DestruirVector(&vec);
                }
                else
                {
                    printf("Error: No se pudo leer el binario. Ejecutar el Punto 8 primero.\n");
                }
            }
            break;
        case 11:
            printf("\nEjecutando Punto 11...\n");
            {
                Vector vec;
                if (CrearVector(&vec, sizeof(tRegModif), 10000) && bajartxt(archivoBinario, &vec) == 0)
                {
                    OrdenarSeleccion(&vec, cmpNivelSexoTipo);
                    Punto_11(&vec);
                    DestruirVector(&vec);
                }
                else
                {
                    printf("Error: No se pudo leer el binario. Ejecutar el Punto 8 primero.\n");
                }
            }
            break;
        case 0:
            printf("\nSaliendo del programa.\n");
            break;
        default:
            printf("\nOpcion no valida. Por favor, ingrese un numero del 0 al 11.\n");
        }

        // Si no eligió salir, mete una pausa antes de volver a evaluar el ciclo
        if (opcion != 0)
        {
            printf("\n");
            system("pause");
        }

    }
    while(opcion != 0);

    return 0;
}

