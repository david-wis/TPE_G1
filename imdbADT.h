//
// Created by david on 12/3/21.
//

#ifndef TPE_G1_IMDBADT_H
#define TPE_G1_IMDBADT_H
#include <stdlib.h>
#include "title.h"

typedef struct imdbCDT * imdbADT;

imdbADT newImdbADT(char ** genres, size_t qtyGenres);

void loadTitleByYear(imdbADT imdb, tTitle * t);

void loadTitleByTypeGenre(imdbADT imdb, tTitle * t);

void loadTopMovie(imdbADT imdb, tTitle * t);

#endif //TPE_G1_IMDBADT_H
