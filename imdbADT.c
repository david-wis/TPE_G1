//
// Created by david on 12/3/21.
//

#include "imdbADT.h"

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"

typedef struct tMovieNode * tMovieList;

typedef struct tMovieNode {
    char * film;
    unsigned long votes;
    float rating;
    signed char * genres; // Vector -1 terminated de generos
    tMovieList nextM;
} tMovieNode;

typedef struct tGenreQtys {
    unsigned long qtyFilms;
    unsigned long qtySeries;
} tGenreQtys;

typedef struct tYearNode * tYearList;

typedef struct tYearNode {
    unsigned short year;
    unsigned long qtyFilms;
    unsigned long qtySeries;
    unsigned long qtyShorts;
    tGenreQtys * qtyByGenre; // Vector de genero
    tMovieList firstM;
} tYearNode;

typedef struct imdbCDT {
    char ** genres;
    size_t qtyGenres;
    tYearList firstY;
} imdbCDT;

imdbADT newImdbADT(char ** genres, size_t qtyGenres) {
    imdbADT imdb = safeMalloc(sizeof(imdbCDT));
    imdb->genres = genres;
    imdb->qtyGenres = qtyGenres;
    imdb->firstY = NULL;
    return imdb;
}

