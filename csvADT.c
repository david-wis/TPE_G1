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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csvADT.h"
#include "utils.h"
#include "title.h"

#define BUFFER_SIZE 256                         // Tamaño de buffer de lectura de línea
#define FILE_ERROR "Could not open file\n"      // Mensaje de error en caso de fallo de apertura de archivo
#define MODE_ERROR "Operation not permitted\n"  // Mensaje de error en caso de intento de operación no permitida por el modo con el que se abrió el archivo
#define DELIM ";"                               // Cadena de delimitación de los campos del csv
#define EMPTY "\\N"                             // Cadena que representa valor nulo en los archivos

/*!
 * Retorna la siguiente linea de texto de un archivo
 * @param f Archivo a leer
 * @return String de la linea, NULL si es vacía
 */
static char * readLine(FILE * f);

/*!
 * Verifica si el modo (de lectura o escritura) es el esperado, y si no, aborta
 * @param expected Modo esperado
 * @param current Modo recibido
 */
static void checkMode(tMode expected, tMode current);

/*!
 * Parsea un token a int
 * @param s String que se desea tokenizar o null si ya se esta tokenizando
 * @return Texto convertido a entero largo, 0 si se lee la cadena nula
 */
static unsigned long parseInt(char * s);

/*!
 * Parsea un token a int
 * @param s String que se desea tokenizar o null si ya se esta tokenizando
 * @return Texto convertido a punto flotante, 0 si se lee la cadena nula
 */
static float parseFloat(char * s);


/*!
 * Crea un espacio de memoria y le copia el string del token
 * @param s String que se desea tokenizar o null si ya se esta tokenizando
 * @return Puntero a la copia del string en el heap, NULL si se lee la cadena nula
 */
static char * parseString(char * s);

/*!
 * Parsea el tipo de un titulo
 * @param s String que se desea tokenizar o null si ya se esta tokenizando
 * @param titleTypes Texto de los titulos disponibles
 * @param typesDim Dimension del vector de nombres de titulos
 * @param error Devuelve 0 si encontro el titulo y distinto de cero si no
 * @return Indice del vector que se corresponde al titulo encontrado
 */
static int parseType(char * s, char * titleTypes[], size_t typesDim, int * error);

/*!
 * Parsea una lista de strings, prendiendo los bits de un int si encuentra que el genero esta presente
 * @param s String que se desea tokenizar o null si ya se esta tokenizando
 * @param genres Vector de generos disponibles
 * @param genresDim Dimension del vector de nombres de generos
 * @return Un entero con los bits prendidos de acuerdo a los generos que aparecen en el vector
 */
static unsigned int parseGenres(char * s, char ** genres, size_t genresDim);

typedef struct csvCDT { //CDT de manejo de un archivo
    FILE * file;    // Puntero a archivo
    size_t line;    // Número de línea leído
    tMode mode;     // Modo de apertura del archivo
} csvCDT;

static char * readLine(FILE * f) {
    char * c = safeMalloc(BUFFER_SIZE);
    size_t len = 0;
    char * p;
    do {    // Lee hasta terminar la línea
       p = fgets(c + len, BUFFER_SIZE-1, f);
       if (p != NULL) {
           len += strlen(c + len);
           c = safeRealloc(c, len + BUFFER_SIZE);
       }
    } while(p != NULL && len > 0 && c[len-1] != '\n');

    if (!len || c[0] == '\n') { // En caso de leer una línea vacía, retorna NULL
        free(c);
        return NULL;
    }

    if (c[len-1]=='\n') {   // Cortar caracter salto de línea
        c = safeRealloc(c, len);
        c[len-1] = 0;
    } else  {
        c = safeRealloc(c, len + 1);
        c[len] = 0;
    }
    return c;
}

static void checkMode(tMode expected, tMode current) {
   if (expected != current) {
       fprintf(stderr, MODE_ERROR);
       exit(1);
   }
}

csvADT newCsv(char * path, tMode mode) {
    static char * stringModes[] = {"r", "w"};
    csvADT csv = safeMalloc(sizeof(csvCDT));
    csv->file = fopen(path, stringModes[mode]);
    csv->mode = mode;
    if (csv->file == NULL) {
        fprintf(stderr, FILE_ERROR);
        exit(1);
    }
    csv->line = 0;
    return csv;
}

void closeFile(csvADT csv){
    fclose(csv->file);
    free(csv);
}

char * readNextString(csvADT csv) {
    checkMode(READ, csv->mode);
    if (csv->line == 0) {
        free(readLine(csv->file)); // Leemos la primera linea sin guardarla
        csv->line++;
    }
    csv->line++;
    return readLine(csv->file);
}

static unsigned long parseInt(char * s) {
    char * token = strtok(s, DELIM);
    if (strcmp(token, EMPTY) == 0)
        return 0;
    return atol(token);
}

static float parseFloat(char * s){
    char * token = strtok(s, DELIM);
    if (strcmp(token, EMPTY) == 0)
        return 0.0f;
    return atof(token);
}

static char * parseString(char * s) {
    char * token = strtok(s, DELIM);
    if (strcmp(token, EMPTY) == 0)
        return NULL;
    char * copy = safeMalloc(strlen(token) + 1);
    return strcpy(copy, token);
}

static int parseType(char * s, char * titleTypes[], size_t typesDim, int * error) { // Error devuelve 0 si no hay error y distinto de cero si hay
    int cmp = 1, i;
    char * token = strtok(s, DELIM);
    for (i = 0; i < typesDim && (cmp = strcmp(token, titleTypes[i])) > 0; i++)
        ;
    *error = cmp;
    return i;
}

static unsigned int parseGenres(char * s, char ** genres, size_t genresDim) {
    char * token = strtok(s, DELIM);
    char * p;
    unsigned int state = 0;
    size_t len, totalLen = strlen(token);
    for (size_t i = 0; i < genresDim && totalLen > 0; ++i) {    // Si ya se leyeron todos los géneros del título, corta el ciclo
        p = strstr(token, genres[i]);
        if (p != NULL) {
            len = strlen(genres[i]);
            if (p[len] == '\0' || p[len] == ',') {  // Checkea si lo encontrado es una coincidencia completa y no simplemente una subcadena
                totalLen -= (len + (p[len] == ','));
                state |= 1<<i;
            }
        }
    }
    return state;
}

tTitle * readNextTitle(csvADT csv, char ** genres, size_t genresDim,
                        char * titleTypes[], size_t typesDim) {
    char * line = readNextString(csv);
    if (line == NULL)   // Si la linea está vacía, se ignora
        return NULL;

    tTitle * title = safeMalloc(sizeof(tTitle));
    int error;

    title->id = parseString(line);
    title->titleType = parseType(NULL, titleTypes, typesDim, &error);
    if (error) {    // Si el tipo no se encuentra en los preestablecidos, se ignora
        free(title->id);
        free(title);
        free(line);
        return NULL;
    }

    title->primaryTitle = parseString(NULL);
    title->startYear = parseInt(NULL);
    if (title->startYear == 0)  {   // Si no tiene año de comienzo se ignora
        freeTitle(title);
        free(line);
        return NULL;
    }

    title->endYear = parseInt(NULL);
    title->genres = parseGenres(NULL, genres, genresDim);
    title->avgRating = parseFloat(NULL);
    title->numVotes = parseInt(NULL);
    if(title->numVotes == 0){   // Si tiene 0 votos se ignora
        freeTitle(title);
        free(line);
        return NULL;
    }

    title->runtimeMinutes = parseInt(NULL);
    free(line);
    return title;
}

int eof(csvADT csv) {
    checkMode(READ, csv->mode);
    return feof(csv->file);
}

void writeQuery1(csvADT csv, unsigned short year, unsigned long films, unsigned long series, unsigned long shorts) {
    checkMode(WRITE, csv->mode);
    fprintf(csv->file, "%d;%lu;%lu;%lu\n", year, films, series, shorts);
}

void writeQuery2(csvADT csv, unsigned short year, unsigned char genreDim, char ** genres, unsigned long ** qtyByGenres){
    checkMode(WRITE, csv->mode);
    for (int i = 0; i < genreDim; i++) {
        if(qtyByGenres[0][i] || qtyByGenres[1][i]) {
            fprintf(csv->file, "%d;%s;%lu;%lu\n", year, genres[i], qtyByGenres[0][i], qtyByGenres[1][i]);
        }
    }
}

void writeQuery3(csvADT csv, unsigned short year, const char * title, unsigned long votes, float rating, const char * genres) {
    checkMode(WRITE, csv->mode);
    fprintf(csv->file, "%d;%s;%lu;%0.2f;%s\n", year, title, votes, rating, genres);
}

void writeString(csvADT csv, const char * txt) {
    checkMode(WRITE, csv->mode);
    fprintf(csv->file, "%s\n", txt);
}
