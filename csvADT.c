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

typedef struct csvCDT {
    FILE * file;
    size_t line;
} csvCDT;

static char * readLine(FILE * f) {
    char * c = safeMalloc(BUFFER_SIZE);
    size_t len = 0;
    while ((fgets(c + len, BUFFER_SIZE-1, f)) != NULL) {
        len += strlen(c + len);
        c = safeRealloc(c, len + BUFFER_SIZE);
    }
    c = safeRealloc(c, len);
    fclose(f);
    if (len > 0)
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

int closeFile(csvADT csv){
    return fclose(csv->file);
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
    return atol(strtok(s, DELIM));
}

static float parseFloat(char * s){
    return atof(strtok(s, DELIM));
}

static char * parseString(char * s) {
    char * token = strtok(s, DELIM);
    char * copy = safeMalloc(strlen(token) + 1);
    return strcpy(copy, token);
}

/*!
 * @param csv: ADT de csv
 * @param genres: Vector NULL terminated con nombres de los generos
 * @param titleTypes: Vector NULL terminated con nombres de los tipos de titulos
 * @return
 */
tTitle * readNextTitle(csvADT csv, char ** genres, char ** titleTypes) {
    tTitle * title = safeMalloc(sizeof(tTitle));
    char * line = readNextString(csv);
    char * token = strtok(line, DELIM);
    title->runtimeMinutes = parseInt(NULL);
    return title;
}

