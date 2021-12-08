#include <stdio.h>
#include <stdlib.h>
#include "csvADT.h"
#include "imdbADT.h"

#define QTY_GENRES 32
#define WRONG_PARAMS "Two arguments were expected"

void loadGenres(char * fileName, char * vecGenres[], size_t * genreDim);

int main(int argc, char * argv[]) {
    #ifdef RELEASE
    if (argc != 2) {
        fprintf(stderr, WRONG_PARAMS);
        exit(1);
    }
    #endif

    char * titleTypes[4] = {"movie", "short", "tvMiniSeries", "tvSeries"};
    size_t genreDim;
    char * vecGenres[QTY_GENRES];
    loadGenres("/Users/mbp13/Desktop/PI/TPE_G1/genres.csv", vecGenres, &genreDim); //TODO: Ponerlo con argv

    imdbADT imdb = newImdbADT(vecGenres, genreDim);
    csvADT csvTitles = newCsv("/Users/mbp13/Desktop/PI/TPE_G1/imdb.csv", "r"); //TODO: Ponerlo con argv
    tTitle * title;
    while (!eof(csvTitles)) {
        title = readNextTitle(csvTitles, vecGenres, titleTypes);
        loadTitleByYear(imdb, title);
        loadTitleByTypeGenre(imdb, title);
        loadTopMovie(imdb, title);

    }
    return 0;
}

void loadGenres(char * fileName, char * vecGenres[], size_t * genreDim) {
    csvADT genresCsv = newCsv(fileName, "r");
    size_t i = 0;
    while (i < QTY_GENRES && !eof(genresCsv))
        vecGenres[i++] = readNextString(genresCsv);
    *genreDim = i > 0? i - 1 : i;
    closeFile(genresCsv);
}