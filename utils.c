/*
 *  Fecha: 14 de diciembre de 2021
 *  Autores: Liu, Jonathan Daniel
 *           Vilamowski, Abril
 *           Wischñevsky, David
 *  Version: 1.0
 *
 *  Funciones auxiliares de asignación y reasignación de memoria con control de errores de falta de memoria
 */

#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define ERR_MSG "Not enough memory\n"       // Mensaje de error en caso de error de falta de memoria

static void checkInvalidMemory(void * m) {
    if (errno == ENOMEM || m == NULL) {
        perror(ERR_MSG);
        exit(1);
    }
}

void * safeMalloc(size_t bytes) {
    void * m = malloc(bytes);
    checkInvalidMemory(m);
    return m;
}

void * safeCalloc(size_t size, size_t bytes){
    void * m = calloc(size, bytes);
    checkInvalidMemory(m);
    return m;
}

void * safeRealloc(void * p, size_t bytes){
    void * m = realloc(p, bytes);
    checkInvalidMemory(m);
    return m;
}
