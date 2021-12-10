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
#define FILE_ERROR "Could not open file\n"
#define MODE_ERROR "Operation not permitted\n"
#define DELIM ";"
#define EMPTY "\\N"

typedef struct csvCDT {
    FILE * file;
    size_t line;
    tMode mode;
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

    if (!len || (len == 1 && c[0] == '\n')) {
        free(c);
        return NULL;
    }

    if (c[len-1]=='\n') {
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
    char * stringModes[] = {"r", "w"};
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
    for (i = 0; i < typesDim && (cmp = strcmp(token, titleTypes[i])) > 0; i++)  // Ver si conviene hacerlo generico en utils
        ;
    *error = cmp;
    return i;
}

static unsigned int parseGenres(char * s, char ** genres, size_t genresDim) {
    char * token = strtok(s, DELIM);
    char * p;
    unsigned int state = 0;
    size_t len, totalLen = strlen(token);
    for (size_t i = 0; i < genresDim && totalLen > 0; ++i) {
        p = strstr(token, genres[i]);
        if (p != NULL) {
            len = strlen(genres[i]);
            if (p[len] == '\0' || p[len] == ',') {
                totalLen -= (len + (p[len] == ','));
                state |= 1<<i;
            }
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
    char * line = readNextString(csv);
    if (line == NULL)
        return NULL;

    tTitle * title = safeMalloc(sizeof(tTitle));
    int error;

    title->id = parseString(line);
    title->titleType = parseType(NULL, titleTypes, typesDim, &error);
    if (error) {
        free(title->id);
        free(title);
        free(line);
        return NULL;
    }

    title->primaryTitle = parseString(NULL);
    title->startYear = parseInt(NULL);
    if (title->startYear == 0)  {
        freeTitle(title);
        free(line);
        return NULL;
    }

    title->endYear = parseInt(NULL);
    title->genres = parseGenres(NULL, genres, genresDim);
    title->avgRating = parseFloat(NULL);
    title->numVotes = parseInt(NULL);
    if(title->numVotes == 0){
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
        if(qtyByGenres[0][i] || qtyByGenres[1][i])
            fprintf(csv->file, "%d;%s;%lu;%lu\n", year, genres[i], qtyByGenres[0][i], qtyByGenres[1][i]);
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
