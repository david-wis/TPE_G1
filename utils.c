//
// Created by david on 12/3/21.
//

#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

#define ERR_MSG "Not enough memory"

static void checkInvalidMemory(void * m) {
    if (m == NULL) {
        fprintf(stderr, ERR_MSG);
        exit(1);
    }
}

void * safeMalloc(size_t bytes) {
    void * m = malloc(bytes);
    checkInvalidMemory(m);
    return m;
}

void * safeCalloc(size_t bytes, size_t size){
    void * m = calloc(size, bytes);
    checkInvalidMemory(m);
    return m;
}

void * safeRealloc(size_t bytes, void * p){
    void * m = realloc(p, bytes);
    checkInvalidMemory(m);
    return m;
}