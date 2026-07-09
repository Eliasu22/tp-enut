# Análisis de Datos ENUT 2021 - Procesamiento en C

Este proyecto es una aplicación desarrollada en lenguaje C para el procesamiento, filtrado y análisis de la base de datos de la **Encuesta Nacional de Uso del Tiempo (ENUT 2021)**, elaborada por el INDEC. 

El programa se encarga de leer grandes volúmenes de datos desde archivos de texto plano (`.csv`), realizar cálculos estadísticos aplicando filtros específicos y gestionar el almacenamiento en archivos binarios para optimizar el rendimiento.

## 🚀 Funcionalidades Principales

* **Parseo de archivos CSV:** Lectura y extracción de datos mediante aritmética de punteros.
* **Procesamiento de Estadísticas:** Cálculo de totales de hogares (`WHOG`) y personas (`WPER`) ponderadas, con filtros dinámicos por región y sexo.
* **Gestión de Archivos Binarios:** Conversión del dataset original a formato binario (`.bin`).
* **Memoria Dinámica (TDA Vector):** Carga masiva de registros en la memoria utilizando un Tipo de Dato Abstracto (TDA Vector Dinámico) para su posterior ordenamiento.

## 🛠️ Tecnologías y Conceptos Aplicados

* **Lenguaje:** C
* **Manejo de Memoria:** Memoria dinámica (`malloc`, `realloc`, `free`), manejo de *memory leaks*.
* **Estructuras de Datos:** Vectores dinámicos, `structs`, arrays.
* **Algoritmos:** Punteros, manipulación strings (sin depender exclusivamente de la libreria `<string.h>`).

## 📁 Estructura del Proyecto

* `main.c`: Punto de entrada del programa y menú interactivo.
* `funciones.c`: Implementación de la lógica de negocio, lectura de archivos y algoritmos de procesamiento.
* `funciones.h`: Cabeceras, declaración de estructuras y prototipos de funciones.
