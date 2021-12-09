//
// Created by david on 12/3/21.
//

#include "imdbADT.h"

#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "title.h"

typedef struct tMovieNode * tMovieList;

typedef struct tMovieNode {
    char * film;
    unsigned long votes;
    float rating;
    unsigned int genres;
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
static void freeMoviesRec(tMovieList movie) {
    if (movie == NULL)
        return;
    freeMoviesRec(movie->nextM);
    free(movie->film);
    free(movie);
}

static void freeYearsRec(tYearList years){
    if(years == NULL)
        return;
    freeMoviesRec(years->firstM);
    free(years->qtyByGenre);
    freeYearsRec(years->nextY);
    free(years);
}

void freeImdb(imdbADT imdb) {
    freeYearsRec(imdb->firstY);
    free(imdb);
}

void loadTitleByYear(imdbADT imdb, tTitle * t) { //QUERY 1

}

void loadTitleByTypeGenre(imdbADT imdb, tTitle * t) { //QUERY 2

}

void loadTopMovie(imdbADT imdb, tTitle * t) { //QUERY 3

}

