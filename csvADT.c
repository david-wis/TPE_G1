//
// Created by mbp13 on 03/12/2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csvADT.h"
#include "utils.h"
#include "title.h"
#define BUFFER_SIZE 256
#define FILE_ERROR "Could not open file"
#define DELIM ";"
#define EMPTY "\\N"

typedef struct csvCDT {
    FILE * file;
    size_t line;
} csvCDT;

static char * readLine(FILE * f) {
    char * c = safeMalloc(BUFFER_SIZE);
    size_t len = 0;
    char * p;
    do {
       p = fgets(c + len, BUFFER_SIZE-1, f);
       if (p != NULL) {
           len += strlen(c + len);
           c = safeRealloc(c, len + BUFFER_SIZE);
       }
    } while(p != NULL && len > 0 && c[len-1] != '\n');
    if (!len)
        return NULL;
    if (c[len-1]=='\n')
        c[len-1] = 0;
    return c;
}

csvADT newCsv(char * path, char * mode) {
    csvADT csv = safeMalloc(sizeof(csvCDT));
    csv->file = fopen(path, mode);
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
    if (csv->line == 0) {
        free(readLine(csv->file)); // Leemos la primera linea sin guardarla
        csv->line++;
    }
    csv->line++;
    return readLine(csv->file);
}

static unsigned long parseInt(char * s) { // TODO: Ver si conviene separarlo
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

static char parseType(char * s, char * titleTypes[], size_t typesDim, int * error) { // Error devuelve 0 si no hay error y distinto de cero si hay
    int cmp = -1;
    char * token = strtok(s, DELIM), i;
    for (i = 0; i < typesDim && (cmp = strcmp(token, titleTypes[i])) < 0; i++)  // Ver si conviene hacerlo generico en utils
        ;
    *error = cmp;
    return i;
}

static unsigned int parseGenres(char * s, char ** genres, size_t genresDim) {
    char * token = strtok(s, DELIM);
    char * p;
    unsigned int state = 0;
    size_t len;
    for (size_t i = 0; i < genresDim; ++i) { // TODO: Ver si se puede optimizar
        p = strstr(token, genres[i]);
        len = strlen(p);
        if (p[len] == '\0' || p[len] == ',') {
            state += 1<<i;
        }
    }
    return state;
}

/*!
 * @param csv: ADT de csv
 * @param genres: Vector NULL terminated con nombres de los generos
 * @param titleTypes: Vector NULL terminated con nombres de los tipos de titulos
 * @return
 */

tTitle * readNextTitle(csvADT csv, char ** genres, size_t genresDim,
                       char * titleTypes[], size_t typesDim) {
    tTitle * title = safeMalloc(sizeof(tTitle));
    char * line = readNextString(csv);
    int error;

    title->id = parseString(line);

    title->titleType = parseType(NULL, titleTypes, typesDim, &error);
    if (error) {
        free(title->id);
        free(title);
        return NULL;
    }

    title->primaryTitle = parseString(NULL);
    title->startYear = parseInt(NULL);
    title->endYear = parseInt(NULL);
    title->genres = parseGenres(NULL, genres, genresDim);
    title->avgRating = parseFloat(NULL);

    title->numVotes = parseInt(NULL);
    if(title->numVotes == 0){
        freeTitle(title);
        return NULL;
    }

    title->runtimeMinutes = parseInt(NULL);
    return title;
}

int eof(csvADT csv) {
     return feof(csv->file);
}
