/*
 *  Fecha: 14 de diciembre de 2021
 *  Autores: Liu, Jonathan Daniel
 *           Vilamowski, Abril
 *           Wischñevsky, David
 *  Version: 1.0
 *
 * ADT encargado del manejo de archivos CSV
 * En un principio se intentó hacer genérico, pero luego se decidió por una variante más específica para el TPE
 * Permite leer líneas de texto de archivos y/o parsearlas en tTitles válidos
 * También permite escribir texto estático o con las plantillas de "Query 1", "Query 2" y "Query 3"
 */

#ifndef TPE_G1_CSVADT_H
#define TPE_G1_CSVADT_H
#include "title.h"

typedef struct csvCDT * csvADT;

typedef enum {READ=0, WRITE} tMode; // Enum de modos de apertura de archivos

/*!
 * Inicializa el ADT de csv
 * @param path Ruta del archivo donde se querrá leer o escribir
 * @param mode Modo (lectura/escritura)
 * @return ADT de csv
 */
csvADT newCsv(char * path, tMode mode);

/*!
 * @param csv ADT de csv
 * @return linea con los datos del titulo sin procesar
 */
char * readNextString(csvADT csv);

/*!
 *
 * @param csv ADT de csv
 * @param genres Vector de géneros
 * @param genresDim Cantidad de géneros
 * @param titleTypes Vector de tipos de título
 * @param typesDim Cantidad de tipos de título
 * @return Estructura con la información de un título, NULL si se debe ignorar
 */
tTitle * readNextTitle(csvADT csv, char ** genres, size_t genresDim, char * titleTypes[], size_t typesDim);

/*!
 *
 * @param csv ADT de csv
 * @return 0 si no terminó el archivo, 1 si terminó
 */
int eof(csvADT csv);

/*!
 * Cierra el archivo y libera el ADT de csv
 * @param csv ADT de csv
 */
void closeFile(csvADT csv);

/*!
 * Permite escribir texto estático
 * @param csv ADT de csv
 * @param txt Línea que se desea escribir en el archivo
 */
void writeString(csvADT csv, const char * txt);

/*!
 * Escribe los datos correspondientes a las estadísticas de la query 1 en el archivo
 * @param csv ADT de csv
 * @param year Año acerca del cual se desea escribir la información
 * @param films Cantidad de películas del año
 * @param series Cantidad de series del año
 * @param shorts Cantidad de cortos del año
 */
void writeQuery1(csvADT csv, unsigned short year, unsigned long films, unsigned long series, unsigned long shorts);

/*!
 * Escribe los datos correspondientes a las estadísticas de la query 2 en el archivo
 * @param csv ADT de csv
 * @param year Año acerca del cual se desea escribir la información
 * @param genreDim Cantidad de géneros
 * @param genres Vector de géneros
 * @param qtyByGenres Matriz de 2xN siendo N la cantidad de géneros que existen.
 *                    En la primera fila se acumula la cantidad de películas y en la segunda la de series.
 */
void writeQuery2(csvADT csv, unsigned short year, unsigned char genreDim, char ** genres, unsigned long ** qtyByGenres);

/*!
 * Escribe los datos correspondientes a las estadísticas de la query 3 en el archivo
 * @param csv ADT de csv
 * @param year Año de salida de la película
 * @param title Título de la película
 * @param votes Cantidad de votos de la película
 * @param rating Rating de la película
 * @param genres Cadena de los nombres de los géneros de la película separados por comas
 */
void writeQuery3(csvADT csv, unsigned short year, const char * title, unsigned long votes, float rating, const char * genres);

#endif //TPE_G1_CSVADT_H
