//
// Created by david on 12/3/21.
//

#ifndef TPE_G1_IMDBADT_H
#define TPE_G1_IMDBADT_H
#include <stdlib.h>
#include "title.h"

typedef struct imdbCDT * imdbADT;

imdbADT newImdbADT(char ** genres, char qtyGenres);

void loadData(imdbADT imdb, tTitle * title);

void freeImdb(imdbADT imdb);

#endif //TPE_G1_IMDBADT_H
