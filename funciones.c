#include "funciones.h"

// =====================================================================
// UTILIDADES GENERALES (Compartidas)
// =====================================================================

const char *nombreRegion(int region)
{
    switch (region)
    {
    case 1:
        return "GBA";
    case 2:
        return "PAMPEANA";
    case 3:
        return "NOROESTE";
    case 4:
        return "NORESTE";
    case 5:
        return "CUYO";
    case 6:
        return "PATAGONIA";
    default:
        return "SIN DATO";
    }
}

const char *nombreSexo(int sexo)
{
    switch (sexo)
    {
    case 1:
        return "Mujer";
    case 2:
        return "Hombre";
    default:
        return "SIN DATO";
    }
}

const char *nombreGrupoEdad(int edad)
{
    if (edad >= 14 && edad <= 29) return "14 a 29 anios";
    if (edad >= 30 && edad <= 64) return "30 a 64 anios";
    if (edad >= 65) return "65 anios y mas";
    return "SIN CLASIFICAR";
}

void quitarComillas(char *texto)
{
    char *pInicio = texto;
    char *pEscribir = texto;

    while (*pInicio != '\0')
    {
        if (*pInicio == '"' || *pInicio == '\r' || *pInicio == '\n')
        {
            pInicio++;
        }
        else
        {
            *pEscribir = *pInicio;
            pEscribir++;
            pInicio++;
        }
    }
    *pEscribir = '\0';
}


int obtenerColumnas(char *header, const char *nombresColumnas[], int indicesColumnas[], int cantidadColumnas)
{
    char *token;
    int indiceActual = 0;
    int i;

    // Inicializar array de indices con aritmetica de punteros estricta
    for (i = 0; i < cantidadColumnas; i++)
    {
        *(indicesColumnas + i) = -1;
    }

    token = strtok(header, " \t\r\n;");
    while (token != NULL)
    {
        quitarComillas(token);

        for (i = 0; i < cantidadColumnas; i++)
        {
            if (strcmp(token, *(nombresColumnas + i)) == 0)
            {
                *(indicesColumnas + i) = indiceActual;
            }
        }

        indiceActual++;
        token = strtok(NULL, " \t\r\n;");
    }

    // Verificar que se encontraron todas las columnas
    for (i = 0; i < cantidadColumnas; i++)
    {
        if (*(indicesColumnas + i) == -1) return 0;
    }

    return 1;
}

int esFaltante(const char *texto)
{
    if (texto == NULL || strcmp(texto, "NA") == 0 || strcmp(texto, "") == 0) return 1;
    return 0;
}

int convertirAEntero(const char *texto, int *valor)
{
    if (esFaltante(texto)) return 0;
    *valor = atoi(texto);
    return 1;
}

int convertirADouble(const char *texto, double *valor)
{
    if (esFaltante(texto)) return 0;
    *valor = atof(texto);
    return 1;
}

//void leerBinario(const char *nombreArch)
//{
//    FILE *arch = fopen(nombreArch, "rb");
//    if(!arch) return;
//    tRegModif reg;
//    printf("\nID\t| WHOG\t| WPER\t| REG\t| SEXO\t| EDU\t| TRABAJO\t\t\t| TIEMPO | VALOR\n");
//    printf("----------------------------------------------------------------------------------------------------------\n");
//    fread(&reg, sizeof(tRegModif), 1, arch);
//    while(!feof(arch))
//    {
//        printf("%d\t| %d\t| %d\t| %d\t| %d\t| %d\t| %-25s\t| %d\t | %d\n", reg.id, reg.whog, reg.wper, reg.region, reg.sexo_sel, reg.nivel_educativo, reg.tipo_trabajo, reg.tiempo, reg.valor);
//        fread(&reg, sizeof(tRegModif), 1, arch);
//    }
//    fclose(arch);
//}

void leerBinario(const char *nombreArch) {
    FILE *arch = fopen(nombreArch, "rb");
    if(!arch) {
        printf("Error: No se pudo abrir el archivo binario para lectura.\n");
        return;
    }

    tRegModif reg;
    int contador = 0;

//    printf("\n--- MUESTRA DE REGISTROS BINARIOS (Primeros 10) ---\n");
    printf("\nID\t  WHOG\t  WPER\t  REG\t SEXO\t  EDUC\t  TRABAJO\t\t\t  TIEMPO  VALOR\n");
    printf("----------------------------------------------------------------------------------------------------------\n");

    fread(&reg, sizeof(tRegModif), 1, arch);

    // Leemos hasta el final del archivo, pero cortamos el bucle si el contador llega a 10
    while(!feof(arch) && contador < 10) {
        printf("%d\t| %d\t| %d\t| %d\t| %d\t| %d\t| %-25s\t| %d\t | %d\n",
               reg.id, reg.whog, reg.wper, reg.region, reg.sexo_sel,
               reg.nivel_educativo, reg.tipo_trabajo, reg.tiempo, reg.valor);

        fread(&reg, sizeof(tRegModif), 1, arch);
        contador++;
    }

    // Si cortamos por el contador y no por el final del archivo, avisamos al usuario
    if(!feof(arch)) {
        printf("----------------------------------------------------------------------------------------------------------\n");
        printf("Visualizamos 10 registros a modo de muestra\n");
    }

    fclose(arch);
}

void BinATexto (const char *nombreArchBin, const char *nombreArchTxt)
{
    FILE *archBin = fopen(nombreArchBin, "rb");
    FILE *archTxt = fopen(nombreArchTxt, "wt");
    if(!archBin || !archTxt) return;
    tRegModif reg;
    fprintf(archTxt, "ID;WHOG;WPER;REGION;SEXO;NIVEL_EDUCATIVO;TIPO_TRABAJO;TIEMPO;VALOR\n");
    fread(&reg, sizeof(tRegModif), 1, archBin);
    while(!feof(archBin))
    {
        fprintf(archTxt, "%d;%d;%d;%d;%d;%d;%s;%d;%d\n", reg.id, reg.whog, reg.wper, reg.region, reg.sexo_sel, reg.nivel_educativo, reg.tipo_trabajo, reg.tiempo, reg.valor);
        fread(&reg, sizeof(tRegModif), 1, archBin);
    }
    fclose(archBin);
    fclose(archTxt);
}

// =====================================================================
// PUNTOS 1 Y 2
// =====================================================================

void inicializarRegiones(ResumenRegion *regiones)
{
    ResumenRegion *regionActual, *regionFinal;
    regionFinal = regiones + CANT_REGIONES;
    for (regionActual = regiones; regionActual < regionFinal; regionActual++)
    {
        regionActual->registros = 0;
        regionActual->hogaresEstimados = 0.0;
        regionActual->personasEstimadas = 0.0;
    }
}

void mostrarResumenRegiones(ResumenRegion regiones[])
{
    ResumenRegion *regionActual, *regionFinal;
    regionFinal = regiones + CANT_REGIONES;
    int numeroRegion;

    printf("\n%-8s %-12s %-12s %-18s %-18s\n", "REGION", "NOMBRE", "REGISTROS", "HOGARES ESTIMADOS", "PERSONAS ESTIMADAS");
    for (regionActual = regiones; regionActual < regionFinal; regionActual++)
    {
        numeroRegion = (int)(regionActual - regiones) + 1;
        printf("%-8d %-12s %-12d %-18.2lf %-18.2lf\n",
               numeroRegion, nombreRegion(numeroRegion), regionActual->registros,
               regionActual->hogaresEstimados, regionActual->personasEstimadas);
    }
}

void resolucionPunto1()
{
    FILE *archivo = fopen("enut2021_base.csv", "r");
    char linea[MAX_LINEA];
    char *token;
    int indice;

    int indicesColumnasPunto1[6];
    const char *nombresColumnasPunto1[6] = {"ID", "WHOG", "WPER", "REGION", "SEXO_SEL", "EDAD_SEL"};
    int region;
    double whog, wper;
    ResumenRegion regiones[CANT_REGIONES];
    ResumenRegion *regionActual;

    if (archivo == NULL)
    {
        printf("No se pudo abrir el archivo.\n");
        return;
    }
    if (fgets(linea, MAX_LINEA, archivo) == NULL)
    {
        fclose(archivo);
        return;
    }
    if (!obtenerColumnas(linea, nombresColumnasPunto1, indicesColumnasPunto1, 6))
    {
        fclose(archivo);
        return;
    }

    inicializarRegiones(regiones);

    while (fgets(linea, MAX_LINEA, archivo) != NULL)
    {
        token = strtok(linea, " \t\r\n;");
        indice = 0;
        region = -1;
        whog = 0.0;
        wper = 0.0;

        while (token != NULL)
        {
            // Reemplazo de subindices por aritmetica de punteros
            if (indice == *(indicesColumnasPunto1 + 3)) convertirAEntero(token, &region);
            else if (indice == *(indicesColumnasPunto1 + 1)) convertirADouble(token, &whog);
            else if (indice == *(indicesColumnasPunto1 + 2)) convertirADouble(token, &wper);

            indice++;
            token = strtok(NULL, " \t\r\n;");
        }

        if (region >= 1 && region <= CANT_REGIONES)
        {
            regionActual = regiones + (region - 1);
            regionActual->registros++;
            regionActual->hogaresEstimados += whog;
            regionActual->personasEstimadas += wper;
        }
    }
    fclose(archivo);
    mostrarResumenRegiones(regiones);
}

void resolucionPunto2()
{
    FILE *archivo = fopen("enut2021_base.csv", "r");
    char linea[MAX_LINEA];
    char *token;
    int indice;

    int indicesColumnasPunto2[9];
    const char *nombresColumnasPunto2[9] =
    {
        "ID", "WHOG", "WPER", "REGION", "SEXO_SEL", "EDAD_SEL",
        "TP_GRANGRUPO_OCUPACIONYAUTOCONSUMO", "TP_GRANGRUPO_TRABAJOTOTAL", "TP_GRANGRUPO_TNR"
    };

    if (archivo == NULL) return;
    if (fgets(linea, MAX_LINEA, archivo) == NULL)
    {
        fclose(archivo);
        return;
    }
    if (!obtenerColumnas(linea, nombresColumnasPunto2, indicesColumnasPunto2, 9))
    {
        fclose(archivo);
        return;
    }

    printf("\n%-8s %-12s %-12s %-12s %-10s %-8s %-28s %-18s %-8s %-18s\n",
           "ID", "WHOG", "WPER", "REGION", "SEXO", "EDAD", "OCUPACION_AUTOCONSUMO", "TRABAJO_TOTAL", "TNR", "GRUPO_EDAD");

    int id, region, sexoSel, edadSel, tpOcupacionAutoconsumo, tpTrabajoTotal, tpTnr;
    double whog, wper;

    while (fgets(linea, MAX_LINEA, archivo) != NULL)
    {
        token = strtok(linea, " \t\r\n;");
        indice = 0;
        id = -1;
        whog = 0.0;
        wper = 0.0;
        region = -1;
        sexoSel = -1;
        edadSel = -1;
        tpOcupacionAutoconsumo = -1;
        tpTrabajoTotal = -1;
        tpTnr = -1;

        while (token != NULL)
        {
            // Reemplazo de subindices por aritmetica de punteros
            if (indice == *(indicesColumnasPunto2 + 0)) convertirAEntero(token, &id);
            else if (indice == *(indicesColumnasPunto2 + 1)) convertirADouble(token, &whog);
            else if (indice == *(indicesColumnasPunto2 + 2)) convertirADouble(token, &wper);
            else if (indice == *(indicesColumnasPunto2 + 3)) convertirAEntero(token, &region);
            else if (indice == *(indicesColumnasPunto2 + 4)) convertirAEntero(token, &sexoSel);
            else if (indice == *(indicesColumnasPunto2 + 5)) convertirAEntero(token, &edadSel);
            else if (indice == *(indicesColumnasPunto2 + 6)) convertirAEntero(token, &tpOcupacionAutoconsumo);
            else if (indice == *(indicesColumnasPunto2 + 7)) convertirAEntero(token, &tpTrabajoTotal);
            else if (indice == *(indicesColumnasPunto2 + 8)) convertirAEntero(token, &tpTnr);

            indice++;
            token = strtok(NULL, " \t\r\n;");
        }

        if(id != -1)
        {
            printf("%-8d %-12.2f %-12.2f %-12s %-10s %-8d %-28d %-18d %-8d %-18s\n",
                   id, whog, wper, nombreRegion(region), nombreSexo(sexoSel), edadSel,
                   tpOcupacionAutoconsumo, tpTrabajoTotal, tpTnr, nombreGrupoEdad(edadSel));
        }
    }
    fclose(archivo);
}


// =====================================================================
// PUNTOS 3 Y 4
// =====================================================================

static void limpiarComillasLocales(char *cadena)
{
    int len = strlen(cadena);
    if (len >= 2 && cadena[0] == '"' && cadena[len - 1] == '"')
    {
        for (int i = 0; i < len - 2; i++) cadena[i] = cadena[i + 1];
        cadena[len - 2] = '\0';
    }
}

//void Puntos3y4(const char* ruta_archivo)
//{
//    FILE *archivo = fopen(ruta_archivo, "r");
//    char linea[MAX_LINEA];
//    char *token;
//    int indice;
//
//    int indicesColumnas[4];
//    const char *nombresColumnas[4] = {"REGION", "WHOG", "TIPO_HOGAR_DCTOTAL", "TIPO_HOGAR_DCPOREDAD"};
//
//    // Matrices de acumulacion Puntos 3 y 4 (Dejamos los corchetes intactos segun la regla)
//    float p3_hogares[4][CANT_REGIONES] = {0};
//    int p4_regs_totales[2][CANT_REGIONES] = {0};
//    float p4_hog_totales[2][CANT_REGIONES] = {0};
//    int p4_regs_edad[3][CANT_REGIONES] = {0};
//    float p4_hog_edad[3][CANT_REGIONES] = {0};
//
//    int i, j, r, d, e;
//    const char *descP3[4] = {"Solo hasta 13 anios", "Solo 14 y mas", "Ambos tipos", "Sin demandantes"};
//    const char *descEdad[3] = {"hasta 13", "de 14 y mas", "ambos grupos etarios"};
//
//    int region, dctotal, dcporedad, es_na_poredad;
//    float whog;
//
//    if (archivo == NULL)
//    {
//        printf("Error: No se pudo abrir %s\n", ruta_archivo);
//        return;
//    }
//
//    if (fgets(linea, MAX_LINEA, archivo) == NULL)
//    {
//        fclose(archivo);
//        return;
//    }
//
//    if (!obtenerColumnas(linea, nombresColumnas, indicesColumnas, 4))
//    {
//        printf("Error: No se encontraron todas las columnas necesarias en Puntos 3 y 4.\n");
//        fclose(archivo);
//        return;
//    }
//
//    while (fgets(linea, MAX_LINEA, archivo) != NULL)
//    {
//        token = strtok(linea, " \t\r\n;");
//        indice = 0;
//        region = -1;
//        dctotal = -1;
//        dcporedad = -1;
//        whog = 0.0;
//        es_na_poredad = 0;
//
//        while (token != NULL)
//        {
//            quitarComillas(token);
//
//            // Aplicamos aritmetica de punteros para el vector de indices
//            if (indice == *(indicesColumnas + 0))
//            {
//                convertirAEntero(token, &region);
//            }
//            else if (indice == *(indicesColumnas + 1))
//            {
//                whog = (float)atof(token);
//            }
//            else if (indice == *(indicesColumnas + 2))
//            {
//                convertirAEntero(token, &dctotal);
//            }
//            else if (indice == *(indicesColumnas + 3))
//            {
//                if (esFaltante(token))
//                {
//                    es_na_poredad = 1;
//                }
//                else
//                {
//                    convertirAEntero(token, &dcporedad);
//                }
//            }
//
//            indice++;
//            token = strtok(NULL, " \t\r\n;");
//        }
//
//        // ACUMULACION DE DATOS (Matrices con corchetes respetadas)
//        if (region >= 1 && region <= CANT_REGIONES)
//        {
//            int col_reg = region - 1;
//            int fila_p3 = -1;
//
//            if (es_na_poredad) fila_p3 = 3;
//            else if (dcporedad >= 1 && dcporedad <= 3) fila_p3 = dcporedad - 1;
//
//            if (fila_p3 != -1)
//            {
//                p3_hogares[fila_p3][col_reg] += whog;
//            }
//
//            if (dctotal == 0 || dctotal == 1)
//            {
//                p4_regs_totales[dctotal][col_reg]++;
//                p4_hog_totales[dctotal][col_reg] += whog;
//            }
//
//            if (dctotal == 1 && dcporedad >= 1 && dcporedad <= 3)
//            {
//                p4_regs_edad[dcporedad - 1][col_reg]++;
//                p4_hog_edad[dcporedad - 1][col_reg] += whog;
//            }
//        }
//    }
//    fclose(archivo);
//
//    // --- IMPRESION DE RESULTADOS ---
//    printf("\n%-20s %-10s %-10s %-10s %-10s %-10s %-10s\n",
//           "tipo_hogar", "GBA", "PAMPEANA", "NOROESTE", "NORESTE", "CUYO", "PATAGONIA");
//    for (i = 0; i < 4; i++)
//    {
//        // Aritmetica de punteros para el arreglo de descripciones
//        printf("%-20s", *(descP3 + i));
//        for (j = 0; j < CANT_REGIONES; j++)
//        {
//            printf(" %-10.0f", p3_hogares[i][j]);
//        }
//        printf("\n");
//    }
//
//    printf("\n%-8s %-20s %-15s %-15s\n", "REGION", "DCTOTAL", "registros", "hogares_est");
//    for (r = 0; r < CANT_REGIONES; r++)
//    {
//        for (d = 0; d < 2; d++)
//        {
//            if (p4_regs_totales[d][r] > 0)
//            {
//                printf("%-8d %-20d %-15d %-15.0f\n", r + 1, d, p4_regs_totales[d][r], p4_hog_totales[d][r]);
//            }
//        }
//    }
//
//    printf("\n%-8s %-15s %-12s %-12s %-22s %-12s %-10s\n",
//           "REGION", "DCPOREDAD", "registros", "hogares", "Edad", "Totales_dem", "Prop");
//    for (r = 0; r < CANT_REGIONES; r++)
//    {
//        for (e = 0; e < 3; e++)
//        {
//            if (p4_regs_edad[e][r] > 0)
//            {
//                float prop = 0.0;
//                if (p4_hog_totales[1][r] > 0)
//                {
//                    prop = (p4_hog_edad[e][r] / p4_hog_totales[1][r]) * 100.0f;
//                }
//                // Aritmetica de punteros para el arreglo de descripciones
//                printf("%-8d %-15d %-12d %-12.0f %-22s %-12.0f %-10.2f\n",
//                       r + 1, e + 1, p4_regs_edad[e][r], p4_hog_edad[e][r], *(descEdad + e), p4_hog_totales[1][r], prop);
//            }
//        }
//    }
//
//    printf("\n%-25s %-10s %-10s %-10s %-10s %-10s %-10s\n",
//           "Edad", "GBA", "PAMPEANA", "NOROESTE", "NORESTE", "CUYO", "PATAGONIA");
//    for (e = 0; e < 3; e++)
//    {
//        // Aritmetica de punteros para el arreglo de descripciones
//        printf("%-25s", *(descEdad + e));
//        for (r = 0; r < CANT_REGIONES; r++)
//        {
//            float prop = 0.0;
//            if (p4_hog_totales[1][r] > 0)
//            {
//                prop = (p4_hog_edad[e][r] / p4_hog_totales[1][r]) * 100.0f;
//            }
//            printf(" %-10.2f", prop);
//        }
//        printf("\n");
//    }
//}

void Punto3(const char* ruta_archivo)
{
    FILE *archivo = fopen(ruta_archivo, "r");
    char linea[MAX_LINEA];
    char *token;
    int indice;

    int indicesColumnas[3];
    const char *nombresColumnas[3] = {"REGION", "WHOG", "TIPO_HOGAR_DCPOREDAD"};

    float p3_hogares[4][CANT_REGIONES] = {0};
    int i, j;
    const char *descP3[4] = {"Solo hasta 13 anios", "Solo 14 y mas", "Ambos tipos", "Sin demandantes"};

    int region, dcporedad, es_na_poredad;
    float whog;

    if (archivo == NULL)
    {
        printf("Error: No se pudo abrir %s\n", ruta_archivo);
        return;
    }

    if (fgets(linea, MAX_LINEA, archivo) == NULL)
    {
        fclose(archivo);
        return;
    }
    if (!obtenerColumnas(linea, nombresColumnas, indicesColumnas, 3))
    {
        printf("Error: Faltan columnas para el Punto 3.\n");
        fclose(archivo);
        return;
    }

    while (fgets(linea, MAX_LINEA, archivo) != NULL)
    {
        token = strtok(linea, " \t\r\n;");
        indice = 0;
        region = -1;
        dcporedad = -1;
        whog = 0.0;
        es_na_poredad = 0;

        while (token != NULL)
        {
            quitarComillas(token);
            if (indice == *(indicesColumnas + 0)) convertirAEntero(token, &region);
            else if (indice == *(indicesColumnas + 1)) whog = (float)atof(token);
            else if (indice == *(indicesColumnas + 2))
            {
                if (esFaltante(token)) es_na_poredad = 1;
                else convertirAEntero(token, &dcporedad);
            }
            indice++;
            token = strtok(NULL, " \t\r\n;");
        }

        if (region >= 1 && region <= CANT_REGIONES)
        {
            int col_reg = region - 1;
            int fila_p3 = -1;

            if (es_na_poredad) fila_p3 = 3;
            else if (dcporedad >= 1 && dcporedad <= 3) fila_p3 = dcporedad - 1;

            if (fila_p3 != -1) p3_hogares[fila_p3][col_reg] += whog;
        }
    }
    fclose(archivo);

    printf("\n--- RESULTADOS PUNTO 3 ---\n");
    printf("%-20s %-10s %-10s %-10s %-10s %-10s %-10s\n",
           "tipo_hogar", "GBA", "PAMPEANA", "NOROESTE", "NORESTE", "CUYO", "PATAGONIA");
    for (i = 0; i < 4; i++)
    {
        printf("%-20s", *(descP3 + i));
        for (j = 0; j < CANT_REGIONES; j++) printf(" %-10.0f", p3_hogares[i][j]);
        printf("\n");
    }
}


// =====================================================================
// PUNTO 4
// =====================================================================
void Punto4(const char* ruta_archivo)
{
    FILE *archivo = fopen(ruta_archivo, "r");
    char linea[MAX_LINEA];
    char *token;
    int indice;

    int indicesColumnas[4];
    const char *nombresColumnas[4] = {"REGION", "WHOG", "TIPO_HOGAR_DCTOTAL", "TIPO_HOGAR_DCPOREDAD"};

    int p4_regs_totales[2][CANT_REGIONES] = {0};
    float p4_hog_totales[2][CANT_REGIONES] = {0};
    int p4_regs_edad[3][CANT_REGIONES] = {0};
    float p4_hog_edad[3][CANT_REGIONES] = {0};

    int r, d, e;
    const char *descEdad[3] = {"hasta 13", "de 14 y mas", "ambos grupos etarios"};

    int region, dctotal, dcporedad;
    float whog;

    if (archivo == NULL)
    {
        printf("Error: No se pudo abrir %s\n", ruta_archivo);
        return;
    }

    if (fgets(linea, MAX_LINEA, archivo) == NULL)
    {
        fclose(archivo);
        return;
    }
    if (!obtenerColumnas(linea, nombresColumnas, indicesColumnas, 4))
    {
        printf("Error: Faltan columnas para el Punto 4.\n");
        fclose(archivo);
        return;
    }

    while (fgets(linea, MAX_LINEA, archivo) != NULL)
    {
        token = strtok(linea, " \t\r\n;");
        indice = 0;
        region = -1;
        dctotal = -1;
        dcporedad = -1;
        whog = 0.0;

        while (token != NULL)
        {
            quitarComillas(token);
            if (indice == *(indicesColumnas + 0)) convertirAEntero(token, &region);
            else if (indice == *(indicesColumnas + 1)) whog = (float)atof(token);
            else if (indice == *(indicesColumnas + 2)) convertirAEntero(token, &dctotal);
            else if (indice == *(indicesColumnas + 3))
            {
                if (!esFaltante(token)) convertirAEntero(token, &dcporedad);
            }
            indice++;
            token = strtok(NULL, " \t\r\n;");
        }

        if (region >= 1 && region <= CANT_REGIONES)
        {
            int col_reg = region - 1;

            if (dctotal == 0 || dctotal == 1)
            {
                p4_regs_totales[dctotal][col_reg]++;
                p4_hog_totales[dctotal][col_reg] += whog;
            }

            if (dctotal == 1 && dcporedad >= 1 && dcporedad <= 3)
            {
                p4_regs_edad[dcporedad - 1][col_reg]++;
                p4_hog_edad[dcporedad - 1][col_reg] += whog;
            }
        }
    }
    fclose(archivo);

    printf("\n--- RESULTADOS PUNTO 4 ---\n");
    printf("%-8s %-20s %-15s %-15s\n", "REGION", "DCTOTAL", "registros", "hogares_est");
    for (r = 0; r < CANT_REGIONES; r++)
    {
        for (d = 0; d < 2; d++)
        {
            if (p4_regs_totales[d][r] > 0)
            {
                printf("%-8d %-20d %-15d %-15.0f\n", r + 1, d, p4_regs_totales[d][r], p4_hog_totales[d][r]);
            }
        }
    }

    printf("\n%-8s %-15s %-12s %-12s %-22s %-12s %-10s\n",
           "REGION", "DCPOREDAD", "registros", "hogares", "Edad", "Totales_dem", "Prop");
    for (r = 0; r < CANT_REGIONES; r++)
    {
        for (e = 0; e < 3; e++)
        {
            if (p4_regs_edad[e][r] > 0)
            {
                float prop = 0.0;
                if (p4_hog_totales[1][r] > 0) prop = (p4_hog_edad[e][r] / p4_hog_totales[1][r]) * 100.0f;
                printf("%-8d %-15d %-12d %-12.0f %-22s %-12.0f %-10.2f\n",
                       r + 1, e + 1, p4_regs_edad[e][r], p4_hog_edad[e][r], *(descEdad + e), p4_hog_totales[1][r], prop);
            }
        }
    }

    printf("\n%-25s %-10s %-10s %-10s %-10s %-10s %-10s\n",
           "Edad", "GBA", "PAMPEANA", "NOROESTE", "NORESTE", "CUYO", "PATAGONIA");
    for (e = 0; e < 3; e++)
    {
        printf("%-25s", *(descEdad + e));
        for (r = 0; r < CANT_REGIONES; r++)
        {
            float prop = 0.0;
            if (p4_hog_totales[1][r] > 0) prop = (p4_hog_edad[e][r] / p4_hog_totales[1][r]) * 100.0f;
            printf(" %-10.2f", prop);
        }
        printf("\n");
    }
}










void resolucionPunto5()
{
    FILE *archivo = fopen("enut2021_base.csv", "r");
    char linea[MAX_LINEA];
    char *token;
    int indice;
    int idCol = -1, whogCol = -1, regionCol = -1, tipoHogarTotalCol = -1, cuidadoHogarCol = -1;

    RegionCuidado regionesP5[CANT_REGIONES];
    RegionCuidado *regActual;
    for (regActual = regionesP5; regActual < regionesP5 + CANT_REGIONES; regActual++)
    {
        regActual->hogaresNinguno = 0.0;
        regActual->hogaresExclusivo = 0.0;
    }

    if (archivo == NULL) return;
    if (fgets(linea, MAX_LINEA, archivo) == NULL)
    {
        fclose(archivo);
        return;
    }

    indice = 0;
    token = strtok(linea, " \t\r\n;");
    while (token != NULL)
    {
        quitarComillas(token);
        if (strcmp(token, "ID") == 0) idCol = indice;
        else if (strcmp(token, "WHOG") == 0) whogCol = indice;
        else if (strcmp(token, "REGION") == 0) regionCol = indice;
        else if (strcmp(token, "TIPO_HOGAR_DCTOTAL") == 0) tipoHogarTotalCol = indice;
        else if (strcmp(token, "CUIDADO_SOLO_HOGAR") == 0) cuidadoHogarCol = indice;
        indice++;
        token = strtok(NULL, " \t\r\n;");
    }

    double whog;
    int region, tipoHogarDcTotal, cuidadoSoloHogar;

    while (fgets(linea, MAX_LINEA, archivo) != NULL)
    {
        token = strtok(linea, " \t\r\n;");
        indice = 0;
        whog = 0.0;
        region = -1;
        tipoHogarDcTotal = -1;
        cuidadoSoloHogar = -1;

        while (token != NULL)
        {
            quitarComillas(token);
            if (indice == whogCol) convertirADouble(token, &whog);
            else if (indice == regionCol) convertirAEntero(token, &region);
            else if (indice == tipoHogarTotalCol) convertirAEntero(token, &tipoHogarDcTotal);
            else if (indice == cuidadoHogarCol) convertirAEntero(token, &cuidadoSoloHogar);
            indice++;
            token = strtok(NULL, " \t\r\n;");
        }

        if (tipoHogarDcTotal == 1 && region >= 1 && region <= CANT_REGIONES)
        {
            regActual = regionesP5 + (region - 1);
            if (cuidadoSoloHogar == 0) regActual->hogaresNinguno += whog;
            else if (cuidadoSoloHogar == 1) regActual->hogaresExclusivo += whog;
        }
    }
    fclose(archivo);

    printf("\n%-45s %-10s %-10s %-10s %-10s %-10s %-10s\n", "CUIDADO_SOLO_HOGAR", "GBA", "PAMPEANA", "NOROESTE", "NORESTE", "CUYO", "PATAGONIA");
    printf("%-45s", "Ninguno recibe cuidado exclusivo del hogar");
    for (indice = 0; indice < CANT_REGIONES; indice++)
    {
        regActual = regionesP5 + indice;
        double totalRegion = regActual->hogaresNinguno + regActual->hogaresExclusivo;
        printf(" %9.2lf%%", (totalRegion > 0.0) ? (regActual->hogaresNinguno / totalRegion) * 100.0 : 0.0);
    }
    printf("\n%-45s", "Cuidado exclusivo del propio hogar");
    for (indice = 0; indice < CANT_REGIONES; indice++)
    {
        regActual = regionesP5 + indice;
        double totalRegion = regActual->hogaresNinguno + regActual->hogaresExclusivo;
        printf(" %9.2lf%%", (totalRegion > 0.0) ? (regActual->hogaresExclusivo / totalRegion) * 100.0 : 0.0);
    }
    printf("\n");
}

void acumularDatosTrabajo(FilaReporte *tabla, int indiceFila, double wper, int autoconsumo, int tnr, int trabajoTotal)
{
    FilaReporte *fila = tabla + indiceFila;
    (fila->trabajos + 0)->totalPoblacion += wper;
    if (autoconsumo == 1) (fila->trabajos + 0)->realizanActividad += wper;
    (fila->trabajos + 1)->totalPoblacion += wper;
    if (tnr == 1) (fila->trabajos + 1)->realizanActividad += wper;
    (fila->trabajos + 2)->totalPoblacion += wper;
    if (trabajoTotal == 1) (fila->trabajos + 2)->realizanActividad += wper;
}

void mostrarTablaTrabajo(const FilaReporte *tabla, int cantFilas, const char *nombresFilas[], const char *tituloFila)
{
    const FilaReporte *ptrFila, *ptrFilaTotal;
    const Trabajo *ptrTrabajo;
    const char **ptrNombreFila;
    int j;

    printf("\n%-20s %-25s %-25s %-25s\n", tituloFila, "OCUPACION_Y_AUTOCONS", "TNR", "TRABAJO_TOTAL");
    printf("-------------------------------------------------------------------------------------------------\n");

    ptrNombreFila = nombresFilas;
    for (ptrFila = tabla; ptrFila < tabla + cantFilas; ptrFila++)
    {
        printf("%-20s", *ptrNombreFila);
        for (ptrTrabajo = ptrFila->trabajos; ptrTrabajo < ptrFila->trabajos + CANT_TRABAJOS; ptrTrabajo++)
        {
            double porcentaje = (ptrTrabajo->totalPoblacion > 0.0) ? (ptrTrabajo->realizanActividad / ptrTrabajo->totalPoblacion) * 100.0 : 0.0;
            printf(" %24.2lf%%", porcentaje);
        }
        printf("\n");
        ptrNombreFila++;
    }

    printf("-------------------------------------------------------------------------------------------------\n%-20s", "Total");
    for (j = 0; j < CANT_TRABAJOS; j++)
    {
        double sumaTotalPob = 0.0, sumaRealizan = 0.0;
        for (ptrFilaTotal = tabla; ptrFilaTotal < tabla + cantFilas; ptrFilaTotal++)
        {
            const Trabajo *trabajoActual = ptrFilaTotal->trabajos + j;
            sumaTotalPob += trabajoActual->totalPoblacion;
            sumaRealizan += trabajoActual->realizanActividad;
        }
        printf(" %24.2lf%%", (sumaTotalPob > 0.0) ? (sumaRealizan / sumaTotalPob) * 100.0 : 0.0);
    }
    printf("\n-------------------------------------------------------------------------------------------------\n\n");
}

int guardarMatrizBinaria(const char *nombreArchivo, const FilaReporte *tabla, int cantFilas)
{
    FILE *binario = fopen(nombreArchivo, "wb");
    if (binario == NULL) return 0;
    for (int i = 0; i < cantFilas; i++)
    {
        double porcentajesFila[CANT_TRABAJOS];
        for (int j = 0; j < CANT_TRABAJOS; j++)
        {
            const Trabajo *celda = &(tabla + i)->trabajos[j];
            *(porcentajesFila + j) = (celda->totalPoblacion > 0.0) ? (celda->realizanActividad / celda->totalPoblacion) * 100.0 : 0.0;
        }
        fwrite(porcentajesFila, sizeof(double), CANT_TRABAJOS, binario);
    }
    fclose(binario);
    return 1;
}

void procesarPuntoTrabajo(const char *nombreVariableFiltro, int *pVarFiltroCol, FilaReporte *tablaDestino, int cantFilasDestino)
{
    FILE *archivo = fopen("enut2021_base.csv", "r");
    char linea[MAX_LINEA];
    char *token;
    int indice, idCol = -1, wperCol = -1, tpAutoconsumoCol = -1, tpTrabajoTotalCol = -1, tpTnrCol = -1;
    *pVarFiltroCol = -1;

    if (archivo == NULL) return;
    if (fgets(linea, MAX_LINEA, archivo) == NULL)
    {
        fclose(archivo);
        return;
    }

    indice = 0;
    token = strtok(linea, " \t\r\n;");
    while (token != NULL)
    {
        quitarComillas(token);
        if (strcmp(token, "ID") == 0) idCol = indice;
        else if (strcmp(token, "WPER") == 0) wperCol = indice;
        else if (strcmp(token, nombreVariableFiltro) == 0) *pVarFiltroCol = indice;
        else if (strcmp(token, "TP_GRANGRUPO_OCUPACIONYAUTOCONSUMO") == 0) tpAutoconsumoCol = indice;
        else if (strcmp(token, "TP_GRANGRUPO_TRABAJOTOTAL") == 0) tpTrabajoTotalCol = indice;
        else if (strcmp(token, "TP_GRANGRUPO_TNR") == 0) tpTnrCol = indice;
        indice++;
        token = strtok(NULL, " \t\r\n;");
    }

    double wper;
    int variableFiltro, tpAutoconsumo, tpTrabajoTotal, tpTnr;

    while (fgets(linea, MAX_LINEA, archivo) != NULL)
    {
        token = strtok(linea, " \t\r\n;");
        indice = 0;
        wper = 0.0;
        variableFiltro = -1;
        tpAutoconsumo = -1;
        tpTrabajoTotal = -1;
        tpTnr = -1;

        while (token != NULL)
        {
            quitarComillas(token);
            if (indice == wperCol) convertirADouble(token, &wper);
            else if (indice == *pVarFiltroCol) convertirAEntero(token, &variableFiltro);
            else if (indice == tpAutoconsumoCol) convertirAEntero(token, &tpAutoconsumo);
            else if (indice == tpTrabajoTotalCol) convertirAEntero(token, &tpTrabajoTotal);
            else if (indice == tpTnrCol) convertirAEntero(token, &tpTnr);
            indice++;
            token = strtok(NULL, " \t\r\n;");
        }

        int filaDestino = -1;
        if (strcmp(nombreVariableFiltro, "SEXO_SEL") == 0)
        {
            if (variableFiltro == 1 || variableFiltro == 2) filaDestino = variableFiltro - 1;
        }
        else
        {
            if (variableFiltro >= 14 && variableFiltro <= 29) filaDestino = 0;
            else if (variableFiltro >= 30 && variableFiltro <= 64) filaDestino = 1;
            else if (variableFiltro >= 65) filaDestino = 2;
        }

        if (filaDestino != -1) acumularDatosTrabajo(tablaDestino, filaDestino, wper, tpAutoconsumo, tpTnr, tpTrabajoTotal);
    }
    fclose(archivo);
}

void resolucionPunto6()
{
    int sexoSelCol = -1;
    FilaReporte tablaP6[2];
    for (int i=0; i<2; i++)
    {
        for (int j=0; j<CANT_TRABAJOS; j++)
        {
            tablaP6[i].trabajos[j].totalPoblacion = 0.0;
            tablaP6[i].trabajos[j].realizanActividad = 0.0;
        }
    }

    procesarPuntoTrabajo("SEXO_SEL", &sexoSelCol, tablaP6, 2);
    const char *nombresFilasP6[2] = {"Mujer", "Hombre"};
    printf("\n==================================================================================================================================");
    printf("\nPORCENTAJE DE LA POBLACION QUE REALIZA CADA TIPO DE TRABAJO SEGUN SEXO (PUNTO 6)\n");
    printf("==================================================================================================================================\n");
    mostrarTablaTrabajo(tablaP6, 2, nombresFilasP6, "SEXO_SEL");
    guardarMatrizBinaria("resultados_punto6.dat", tablaP6, 2);
}

void resolucionPunto7()
{
    int grupoEdadSelCol = -1;
    FilaReporte tablaP7[3];
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<CANT_TRABAJOS; j++)
        {
            tablaP7[i].trabajos[j].totalPoblacion = 0.0;
            tablaP7[i].trabajos[j].realizanActividad = 0.0;
        }
    }

    procesarPuntoTrabajo("EDAD_SEL", &grupoEdadSelCol, tablaP7, 3);
    const char *nombresFilasP7[3] = {"14 a 29 anos", "30 a 64 anos", "65 anos y mas"};
    printf("\n==================================================================================================================================");
    printf("\nPORCENTAJE DE LA POBLACION SEGUN TIPO DE TRABAJO POR RANGO ETARIO (PUNTO 7)\n");
    printf("==================================================================================================================================\n");
    mostrarTablaTrabajo(tablaP7, 3, nombresFilasP7, "GRUPO_EDAD_SEL");
}

// =====================================================================
// PUNTO 8
// =====================================================================


void convertirRegistro (tRegistroOrig *regOrig, FILE *archDest)
{
    tRegModif regDest;
    const char *tipos_trabajos[4] = {"Trabajo Total", "Ocupacion y Autoconsumo", "Trabajo No Remunerado", "Actividades Personales"};

    for(int i = 0; i < 4; i++)
    {
        regDest.id = regOrig->id;
        regDest.whog = regOrig->whog;
        regDest.wper = regOrig->wper;
        regDest.region = regOrig->region;
        regDest.sexo_sel = regOrig->sexo;
        regDest.nivel_educativo = regOrig->nivelEducativo;
        regDest.tiempo = regOrig->tcs[i];
        regDest.valor = regOrig->tp[i];
        strcpy(regDest.tipo_trabajo, *(tipos_trabajos + i));

        fwrite(&regDest, sizeof(tRegModif), 1, archDest);
    }
}



void punto_8 (const char *nombreArchOrigen, const char *nombreArchDest)
{
    FILE *archOrig = fopen(nombreArchOrigen, "rt");
    FILE *archDest = fopen(nombreArchDest, "wb");

    if(!archOrig || !archDest)
    {
        printf("Error en la apertura de archivos para el Punto 8.\n");
        return;
    }

    char linea[MAX_LINEA];
    tRegistroOrig reg;
    char *token;
    int indice;
    int posCol[CANT_CAMPOS];

    const char *nombres[CANT_CAMPOS] =
    {
        "ID", "WHOG", "WPER", "REGION", "SEXO_SEL", "NIVEL_EDUCATIVO_AGRUPADO",
        "TCS_GRANGRUPO_TRABAJOTOTAL", "TCS_GRANGRUPO_OCUPACIONYAUTOCONSUMO",
        "TCS_GRANGRUPO_TNR", "TCS_GRANGRUPO_PERSONALES",
        "TP_GRANGRUPO_TRABAJOTOTAL", "TP_GRANGRUPO_OCUPACIONYAUTOCONSUMO",
        "TP_GRANGRUPO_TNR", "TP_GRANGRUPO_PERSONALES"
    };

    // Leer encabezado
    if (fgets(linea, MAX_LINEA, archOrig) == NULL)
    {
        fclose(archOrig);
        fclose(archDest);
        return;
    }

    // Obtener los índices de las columnas usando la función centralizada
    if (!obtenerColumnas(linea, nombres, posCol, CANT_CAMPOS))
    {
        printf("Error: No se encontraron todas las columnas requeridas para el Punto 8.\n");
        fclose(archOrig);
        fclose(archDest);
        return;
    }

    // Recorrido de los registros
    while (fgets(linea, MAX_LINEA, archOrig) != NULL)
    {
        token = strtok(linea, " \t\r\n;");
        indice = 0;

        // Inicializamos la estructura en 0
        reg.id = 0;
        reg.whog = 0;
        reg.wper = 0;
        reg.region = 0;
        reg.sexo = 0;
        reg.nivelEducativo = 0;
        for(int i = 0; i < 4; i++)
        {
            reg.tcs[i] = 0;
            reg.tp[i] = 0;
        }

        while (token != NULL)
        {
            quitarComillas(token);

            if (indice == *(posCol + 0)) convertirAEntero(token, &reg.id);
            else if (indice == *(posCol + 1)) convertirAEntero(token, &reg.whog);
            else if (indice == *(posCol + 2)) convertirAEntero(token, &reg.wper);
            else if (indice == *(posCol + 3)) convertirAEntero(token, &reg.region);
            else if (indice == *(posCol + 4)) convertirAEntero(token, &reg.sexo);
            else if (indice == *(posCol + 5)) convertirAEntero(token, &reg.nivelEducativo);
            else if (indice == *(posCol + 6)) convertirAEntero(token, reg.tcs + 0);
            else if (indice == *(posCol + 7)) convertirAEntero(token, reg.tcs + 1);
            else if (indice == *(posCol + 8)) convertirAEntero(token, reg.tcs + 2);
            else if (indice == *(posCol + 9)) convertirAEntero(token, reg.tcs + 3);
            else if (indice == *(posCol + 10)) convertirAEntero(token, reg.tp + 0);
            else if (indice == *(posCol + 11)) convertirAEntero(token, reg.tp + 1);
            else if (indice == *(posCol + 12)) convertirAEntero(token, reg.tp + 2);
            else if (indice == *(posCol + 13)) convertirAEntero(token, reg.tp + 3);

            indice++;
            token = strtok(NULL, " \t\r\n;");
        }

        convertirRegistro(&reg, archDest);
    }

    fclose(archOrig);
    fclose(archDest);
}

// =====================================================================
// PUNTOS 9, 10 Y 11
// =====================================================================

void intercambiarGen(void *a, void *b, size_t tamElem)
{
    void *aux = malloc(tamElem);
    memcpy(aux,a,tamElem);
    memcpy(a,b,tamElem);
    memcpy(b,aux,tamElem);
    free(aux);
}

void *buscarmenor(void *ini, void *ult, size_t tamElem, Cmp cmp)
{
    char *m = ini;
    char *j;
    for(j = (char*)ini + tamElem; j <= (char*)ult; j += tamElem)
    {
        if(cmp(j, m) < 0) m = j;
    }
    return m;
}

void OrdenarSeleccion(Vector *v, Cmp cmp)
{
    char *ini;
    char *m;
    char *ult = (char*)v->vec + (v->ce - 1) * v->tamElem;
    for(ini = (char*)v->vec; ini < ult; ini += v->tamElem)
    {
        m = buscarmenor(ini, ult, v->tamElem, cmp);
        intercambiarGen(m, ini, v->tamElem);
    }
}

int cmpTipoTrabajo(const void* v1, const void* v2)
{
    const tRegModif* p1 = (const tRegModif*)v1;
    const tRegModif* p2 = (const tRegModif*)v2;
    return strcmp(p1->tipo_trabajo, p2->tipo_trabajo);
}

int cmpNivelSexoTipo(const void* a, const void* b)
{
    const tRegModif* r1 = (const tRegModif*)a;
    const tRegModif* r2 = (const tRegModif*)b;
    int cmp = strcmp(r1->tipo_trabajo, r2->tipo_trabajo);
    if(cmp != 0) return cmp;
    if(r1->sexo_sel != r2->sexo_sel) return r1->sexo_sel - r2->sexo_sel;
    return r1->nivel_educativo - r2->nivel_educativo;
}

bool CrearVector(Vector* v, size_t tamElem, size_t cap)
{
    v->vec=(void*)malloc(tamElem*cap);
    if(!v->vec) return false;
    v->cap=cap;
    v->tamElem=tamElem;
    v->ce=0;
    return true;
}

bool redimensionar(Vector* v, size_t NuevaCap)
{
    void* nVec=(void*)realloc(v->vec,NuevaCap*v->tamElem);
    if(!nVec) return false;
    v->vec=nVec;
    v->cap=NuevaCap;
    return true;
}

void DestruirVector(Vector* v)
{
    free(v->vec);
}

int InsertarAlFinal(Vector* v, void* elem)
{
    if(v->ce==v->cap) if(!redimensionar(v,v->cap*2)) return -1;
    void* PosIns= (char*)v->vec + v->ce * v->tamElem;
    memcpy(PosIns,elem,v->tamElem);
    v->ce++;
    return 0;
}

int bajartxt(const char* nomarch, Vector* vec)
{
    FILE* pf=fopen(nomarch,"rb");
    if(!pf) return -1;
    tRegModif prom;
    fread(&prom,sizeof(tRegModif),1,pf);
    while(!feof(pf))
    {
        InsertarAlFinal(vec,&prom);
        fread(&prom,sizeof(tRegModif),1,pf);
    }
    fclose(pf);
    return 0;
}

void calculos(void* aux, long long* ponderado, long long* wper)
{
    tRegModif* i=(tRegModif*)aux;
    if(i->valor==1)
    {
        (*wper)+=i->wper;
        (*ponderado)+=((long long)i->tiempo*i->wper);
    }
}

int Punto_9(Vector* v)
{
    char* i = (char*)v->vec;
    char* ult = i + (v->ce * v->tamElem);
    char tipo_trabajo[40];
    long long total_ponderado, promedio, total_wper;
    int horas, minutos;

    printf("\n%-30s %s\n", "TIPO TRABAJO", "TIEMPO PROM (HH:MM)");
    while(i < ult)
    {
        strcpy(tipo_trabajo,((tRegModif*)i)->tipo_trabajo);
        total_ponderado=0;
        total_wper=0;
        while(i < ult && strcmp(tipo_trabajo,((tRegModif*)i)->tipo_trabajo)==0)
        {
            calculos(i,&total_ponderado,&total_wper);
            i += v->tamElem;
        }
        if(total_wper>0)
        {
            promedio = total_ponderado/total_wper;
            horas = promedio/60;
            minutos = promedio%60;
        }
        printf("%-30s %6d:%02d\n", tipo_trabajo, horas, minutos);
    }
    return 0;
}

void** crear_matriz(size_t cant_filas, size_t cant_columnas, size_t tamElem)
{
    void** mat = malloc(cant_filas * sizeof(void*));
    if(!mat) return NULL;
    for(size_t i = 0; i < cant_filas; i++)
    {
        mat[i] = malloc(cant_columnas * tamElem);
    }
    return mat;
}

void destruir_matriz(void** mat, size_t filas)
{
    for(size_t i = 0; i < filas; i++) free(mat[i]);
    free(mat);
}

void InicializarCero(int **mat, size_t filas, size_t columnas)
{
    for(size_t i=0; i<filas; i++) for(size_t j=0; j<columnas; j++) mat[i][j]=0;
}

int Punto_10(Vector* v)
{
    char* i = (char*)v->vec;
    char* ult = i + (v->ce * v->tamElem);
    char tipo_trabajo[40];
    long long total_ponderado_hombre, total_ponderado_mujer, promedio_hombre, promedio_mujer, total_wper_mujer, total_wper_hombre;
    int columna=0;

    int** mat = (int**)crear_matriz(2, 4, sizeof(int));
    if(!mat) return -1;
    InicializarCero(mat, 2, 4);

    while(i < ult)
    {
        strcpy(tipo_trabajo,((tRegModif*)i)->tipo_trabajo);
        total_ponderado_mujer=0;
        total_wper_mujer=0;
        total_ponderado_hombre=0;
        total_wper_hombre=0;
        promedio_hombre=0;
        promedio_mujer=0;

        while(i < ult && strcmp(tipo_trabajo,((tRegModif*)i)->tipo_trabajo)==0)
        {
            if(((tRegModif*)i)->sexo_sel==1) calculos(i, &total_ponderado_mujer, &total_wper_mujer);
            else calculos(i, &total_ponderado_hombre, &total_wper_hombre);
            i += v->tamElem;
        }
        if(total_wper_mujer>0) promedio_mujer = total_ponderado_mujer/total_wper_mujer;
        if(total_wper_hombre>0) promedio_hombre = total_ponderado_hombre/total_wper_hombre;

        if (columna < 4)
        {
            mat[0][columna] = promedio_mujer;
            mat[1][columna] = promedio_hombre;
            columna++;
        }
    }

    const char* encabezados[] = {"Actividades Personales", "Ocupacion y Autoconsumo", "Trabajo No Remunerado", "Trabajo Total"};
    printf("\n%-10s", "SEXO");
    for(size_t j = 0; j < 4; j++) printf("%-25s", encabezados[j]);
    printf("\n");

    for(size_t r = 0; r < 2; r++)
    {
        printf("%-10s", r == 0 ? "Mujeres" : "Varones");
        for(size_t c = 0; c < 4; c++)
        {
            printf("%2d:%02d%20s", mat[r][c]/60, mat[r][c]%60, "");
        }
        printf("\n");
    }
    destruir_matriz((void**)mat, 2);
    return 0;
}

int Punto_11(Vector* v)
{
    char* i = (char*)v->vec;
    char* ult = i + (v->ce * v->tamElem);
    char tipo_trabajo[50];
    int sexo, nivel, fila, columna = 0;
    long long total_ponderado, total_wper, promedio;

    int** mat = (int**)crear_matriz(10, 4, sizeof(int));
    if(!mat) return -1;
    InicializarCero(mat, 10, 4);

    while(i < ult)
    {
        strcpy(tipo_trabajo,((tRegModif*)i)->tipo_trabajo);
        fila=0;

        while(i < ult && strcmp(tipo_trabajo,((tRegModif*)i)->tipo_trabajo)==0)
        {
            sexo = ((tRegModif*)i)->sexo_sel;
            while(i < ult && strcmp(tipo_trabajo,((tRegModif*)i)->tipo_trabajo)==0 && sexo == ((tRegModif*)i)->sexo_sel)
            {
                nivel = ((tRegModif*)i)->nivel_educativo;
                total_ponderado=0;
                total_wper=0;
                promedio=0;

                while(i < ult && strcmp(tipo_trabajo,((tRegModif*)i)->tipo_trabajo)==0 && sexo == ((tRegModif*)i)->sexo_sel && nivel == ((tRegModif*)i)->nivel_educativo)
                {
                    calculos(i, &total_ponderado, &total_wper);
                    i += v->tamElem;
                }
                if(total_wper>0) promedio = total_ponderado/total_wper;

                if (fila < 10 && columna < 4)
                {
                    mat[fila][columna] = promedio;
                }
                fila++;
            }
        }
        columna++;
    }

    const char* sexo_arr[] = {"Mujeres", "Mujeres", "Mujeres", "Mujeres", "Mujeres", "Varones", "Varones", "Varones", "Varones", "Varones"};
    const char* nivel_arr[] = {"Hasta primario incompleto", "Primario completo y secundario incompleto", "Secundario completo", "Terciario o universitario incompleto y mas", "Ns/Nc", "Hasta primario incompleto", "Primario completo y secundario incompleto", "Secundario completo", "Terciario o universitario incompleto y mas", "Ns/Nc"};

    printf("\n%-12s %-45s %-15s %-18s %-14s %-14s\n", "SEXO_SEL", "NIVEL_EDUCATIVO", "Act. Personales", "Ocup. y Auto.", "Trabajo No Rem", "Trabajo Total");
    for(size_t r = 0; r < 10; r++)
    {
        printf("%-10s %-45s", sexo_arr[r], nivel_arr[r]);
        for(size_t c = 0; c < 4; c++) printf("%8d:%02d     ", mat[r][c]/60, mat[r][c]%60);
        printf("\n");
    }
    destruir_matriz((void**)mat, 10);
    return 0;
}
