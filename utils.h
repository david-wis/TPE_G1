//
// Created by david on 12/3/21.
//

#ifndef TPE_G1_UTILS_H
#define TPE_G1_UTILS_H
#include <stdio.h>

void * safeMalloc(size_t bytes);

void * safeCalloc(size_t size, size_t bytes);

void * safeRealloc(size_t bytes, void * p);
#endif //TPE_G1_UTILS_H
