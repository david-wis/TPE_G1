//
// Created by david on 12/3/21.
//

#include "imdbADT.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "title.h"
#define MAX_MOVIE_COUNT 5

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
    char topSize;
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

static int compareMovies(tTitle * title, tMovieNode * movie) {
    int aux = (title->numVotes > movie->votes) - (title->numVotes < movie->votes);
    if (!aux)
        aux = strcmp(movie->film, title->primaryTitle);
    return aux;
}

static tMovieList addTopMovieRec(tMovieList list, tTitle * title) {
    int cmp;
    if (list == NULL || (cmp = compareMovies(title, list)) <= 0) {
        tMovieList m = safeMalloc(sizeof(tMovieNode));
        m->film = malloc(strlen(title->primaryTitle) + 1);
        strcpy(m->film, title->primaryTitle);
        m->votes = title->numVotes;
        m->genres = title->genres;
        m->rating = title->avgRating;
        m->nextM = list;
        return m;
    }
    if (cmp > 0)
        list->nextM = addTopMovieRec(list->nextM, title);
    return list;
}

static void loadTopMovie(tYearList year, tTitle * title) { //QUERY 3
    if (year->topSize < MAX_MOVIE_COUNT) { // Si hay menos de 5 peliculas, se guarda siempre como maximo
        year->firstM = addTopMovieRec(year->firstM, title);
        year->topSize++;
    } else if (compareMovies(title, year->firstM) > 0) { // Si hay 5 y es mayor al primero, se borra el menor y se guarda
        tMovieList aux = year->firstM->nextM;
        free(year->firstM->film);
        free(year->firstM);
        year->firstM = aux;
        year->firstM = addTopMovieRec(year->firstM, title);
    }
}

static tYearList insertYearRec(tYearList node, unsigned short year, tYearList * result, char qtyGenres) {
    if (node == NULL || node->year < year) {
        tYearList new = safeMalloc(sizeof(tYearNode));
        new->year = year;
        new->firstM = NULL;
        new->qtyByGenre = safeCalloc(qtyGenres, sizeof(tGenreQtys));
        new->qtyFilms = new->qtySeries = new->qtyShorts = 0;
        new->nextY = node;
        new->topSize = 0;
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
