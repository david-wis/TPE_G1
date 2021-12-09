//
// Created by david on 12/3/21.
//

#include "imdbADT.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utils.h"
#include "title.h"
#define ERR_MSG_OOB "Out of bounds\n"
#define MAX_MOVIE_COUNT 5

typedef struct tMovieNode * tMovieList;

typedef struct tMovieNode {
    char * film;
    unsigned long votes;
    float rating;
    unsigned int genres;
    tMovieList nextM;
} tMovieNode;

typedef struct tYearNode * tYearList;

typedef struct tYearNode {
    unsigned short year;
    unsigned long qtyFilms;
    unsigned long qtySeries;
    unsigned long qtyShorts;
    unsigned long *qtyByGenre[2]; // primer elemento films, segundo series
    char topSize;
    tMovieList firstM;
    tMovieList iterMovie;
    tYearList nextY;
} tYearNode;

typedef struct imdbCDT {
    char ** genres;
    char qtyGenres;
    tYearList firstY;
    tYearList iterYear;
} imdbCDT;


static void checkIterInBounds(imdbADT imdb, int checkIterMovie);
static void freeMoviesRec(tMovieList movie);
static void freeYearsRec(tYearList years);
static void loadTitleByYear(tYearList year, tTitle * title);//Query 1
static void loadTitleByTypeGenre(tYearList year, tTitle * title, char qtyGenres);//Query 2
static void loadTopMovie(tYearList year, tTitle * title);//Query 3
static tYearList insertYearRec(tYearList node, unsigned short year, tYearList * result, char qtyGenres);

imdbADT newImdbADT(char ** genres, char qtyGenres) {
    imdbADT imdb = safeMalloc(sizeof(imdbCDT));
    imdb->genres = genres;
    imdb->qtyGenres = qtyGenres;
    imdb->firstY = NULL;
    imdb->iterYear = NULL;
    return imdb;
}

static void checkIterInBounds(imdbADT imdb, int checkIterMovie) {
    if(imdb->iterYear == NULL || (checkIterMovie && imdb->iterYear->iterMovie == NULL)){
        fprintf(stderr, ERR_MSG_OOB);
        exit(1);
    }
}

//Iterador de peliculas por cada anio
void toBeginMovie(imdbADT imdb){
    checkIterInBounds(imdb, 0);
    imdb->iterYear->iterMovie = imdb->iterYear->firstM;
}

int hasNextMovie(imdbADT imdb) {
    checkIterInBounds(imdb, 0);
    return imdb->iterYear->iterMovie != NULL;
}

void nextMovie(imdbADT imdb){
    checkIterInBounds(imdb, 1);
    imdb->iterYear->iterMovie = imdb->iterYear->iterMovie->nextM;
}

char * getCurrentMovieTitle(imdbADT imdb) {
    checkIterInBounds(imdb, 1);
    return imdb->iterYear->iterMovie->film;
}

unsigned long getCurrentMovieVotes(imdbADT imdb){
    checkIterInBounds(imdb, 1);
    return imdb->iterYear->iterMovie->votes;
}

float getCurrentMovieRaiting(imdbADT imdb){
    checkIterInBounds(imdb, 1);
    return imdb->iterYear->iterMovie->rating;
}

char * getCurrentMovieGenres(imdbADT imdb){
    checkIterInBounds(imdb, 1);
    char * s = NULL;
    size_t len = 0;
    size_t wordlen;
    unsigned int genres = imdb->iterYear->iterMovie->genres;
    if (!genres) {
        s = safeMalloc(3);
        strcpy(s, "\\N");
        return s;
    }
    for (int i = 0; i < imdb->qtyGenres; i++) {
        if (genres & 1 << i) {
            wordlen = strlen(imdb->genres[i]);
            s = safeRealloc(s, len + wordlen + 2);
            strcpy(s + len, imdb->genres[i]);
            s[len + wordlen] = ',';
            len += (wordlen + 1);
        }
    }
    s = safeRealloc(s, len);
    s[len-1] = 0;
    return s;
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
    free(years->qtyByGenre[0]);
    free(years->qtyByGenre[1]);
    freeYearsRec(years->nextY);
    free(years);
}

void freeImdb(imdbADT imdb) {
    freeYearsRec(imdb->firstY);
    free(imdb);
}

//Iterador de anios
void toBeginYear(imdbADT imdb){
    imdb->iterYear = imdb->firstY;
}

int hasNextYear(imdbADT imdb){
    return imdb->iterYear != NULL;
}

void nextYear(imdbADT imdb){
    checkIterInBounds(imdb, 0);
    imdb->iterYear = imdb->iterYear->nextY;
}

unsigned int getCurrentYear(imdbADT imdb){
    checkIterInBounds(imdb, 0);
    return imdb->iterYear->year;
}

unsigned long getQtyFilms(imdbADT imdb){
    checkIterInBounds(imdb, 0);
    return imdb->iterYear->qtyFilms;
}

unsigned long getQtyShorts(imdbADT imdb){
    checkIterInBounds(imdb, 0);
    return imdb->iterYear->qtyShorts;
}

unsigned long getQtySeries(imdbADT imdb){
    checkIterInBounds(imdb, 0);
    return imdb->iterYear->qtySeries;
}

unsigned long ** getQtyByGenresByYear(imdbADT imdb) {
    checkIterInBounds(imdb, 0);
    return imdb->iterYear->qtyByGenre;
}


static void loadTitleByYear(tYearList year, tTitle * title) { //QUERY 1
    switch (title->titleType) {
        case MOVIE:
            year->qtyFilms++;
            break;
        case SHORT:
            year->qtyShorts++;
            break;
        case TVMINISERIES:
        case TVSERIES:
            year->qtySeries++;
            break;
        default:
            break;
    }
}

static void loadTitleByTypeGenre(tYearList year, tTitle * title, char qtyGenres) { //QUERY 2
    for (int i = 0; i < qtyGenres; i++) {
        if(title->genres & 1 << i){
            year->qtyByGenre[0][i] += title->titleType == MOVIE;
            year->qtyByGenre[1][i] += (title->titleType == TVMINISERIES || title->titleType == TVSERIES);
        }
    }
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
        m->film = safeMalloc(strlen(title->primaryTitle) + 1);
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
        new->qtyByGenre[0] = safeCalloc(qtyGenres, sizeof(unsigned long));
        new->qtyByGenre[1] = safeCalloc(qtyGenres, sizeof(unsigned long));
        new->qtyFilms = new->qtySeries = new->qtyShorts = 0;
        new->nextY = node;
        new->topSize = 0;
        new->iterMovie = NULL;
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
        loadTopMovie(pCurrYear, title);
    }
}
