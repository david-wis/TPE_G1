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
    tYearList nextY;
} tYearNode;

typedef struct imdbCDT {
    char ** genres;
    char qtyGenres;
    tYearList firstY;
} imdbCDT;

imdbADT newImdbADT(char ** genres, char qtyGenres) {
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

static void loadTitleByYear(tYearList year, tTitle * title) { //QUERY 1

}

static void loadTitleByTypeGenre(tYearList year, tTitle * title, char qtyGenres) { //QUERY 2

}

static void loadTopMovie(tYearList year, tTitle * title) { //QUERY 3

}

static tYearList insertYearRec(tYearList node, unsigned short year, tYearList * result, char qtyGenres) {
    if (node == NULL || node->year < year) {
        tYearList new = safeMalloc(sizeof(tYearNode));
        new->year = year;
        new->firstM = NULL;
        new->qtyByGenre = safeCalloc(qtyGenres, sizeof(tGenreQtys));
        new->qtyFilms = new->qtySeries = new->qtyShorts = 0;
        new->nextY = node;
        *result = new;
        return new;
    }
    if(node->year > year)
        node->nextY = insertYearRec(node->nextY, year, result, qtyGenres);
    else
        *result = node;
    return node;
}

void loadData(imdbADT imdb, tTitle * title) {
    tYearList pCurrYear;
    imdb->firstY = insertYearRec(imdb->firstY, title->startYear, &pCurrYear, imdb->qtyGenres);

    loadTitleByYear(pCurrYear, title);

    loadTitleByTypeGenre(pCurrYear, title, imdb->qtyGenres);

    if (title->titleType == MOVIE) {
        printf("%s:%d\n", title->primaryTitle, title->startYear);
        loadTopMovie(pCurrYear, title);
    }
}
