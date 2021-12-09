//
// Created by david on 12/3/21.
//

#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define ERR_MSG "Not enough memory\n"

static void checkInvalidMemory(void * m) {
    if (m == NULL) {
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