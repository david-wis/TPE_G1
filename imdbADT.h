//
// Created by david on 12/3/21.
//

#ifndef TPE_G1_IMDBADT_H
#define TPE_G1_IMDBADT_H
#include <stdlib.h>
#include "title.h"

typedef struct imdbCDT * imdbADT;

imdbADT newImdbADT(char ** genres, unsigned char genreDim);

void loadData(imdbADT imdb, tTitle * title);

void freeImdb(imdbADT imdb);

void toBeginYear(imdbADT imdb);

int hasNextYear(imdbADT imdb);

void nextYear(imdbADT imdb);

unsigned long getQtyFilms(imdbADT imdb);

unsigned long getQtyShorts(imdbADT imdb);

unsigned long getQtySeries(imdbADT imdb);

unsigned long ** getQtyByGenresByYear(imdbADT imdb);

unsigned int getCurrentYear(imdbADT imdb);

void toBeginMovie(imdbADT imdb);

int hasNextMovie(imdbADT imdb);

void nextMovie(imdbADT imdb);

char * getCurrentMovieTitle(imdbADT imdb);

unsigned long getCurrentMovieVotes(imdbADT imdb);

float getCurrentMovieRaiting(imdbADT imdb);

char * getCurrentMovieGenres(imdbADT imdb);


#endif //TPE_G1_IMDBADT_H
