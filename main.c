#include <stdio.h>
#include "csvADT.h"
#include "imdbADT.h"
#include <time.h>

#define QTY_GENRES 32
#define WRONG_PARAMS "Two arguments were expected\n"
#define QUERY1_HEADERS "year;films;series;shorts"
#define QUERY2_HEADERS "year;genre;films;series"
#define QUERY3_HEADERS "year;film;votes;rating;genres"
#define QUERY1_FILE "query1.csv"
#define QUERY2_FILE "query2.csv"
#define QUERY3_FILE "query3.csv"
#define QTY_PARAMS 3

void loadGenres(char * fileName, char * vecGenres[], unsigned char * genreDim);
void writeResults(imdbADT imdb, unsigned char genreDim, char ** genres);
void writeMoviesRec(imdbADT imdb, unsigned short year, csvADT csv);
void freeGenres(char ** genres, unsigned char genreDim);

int main(int argc, char * argv[]) {
    if (argc < QTY_PARAMS) {
        fprintf(stderr, WRONG_PARAMS);
        exit(1);
    }

    #if DEBUG
        clock_t t;
        t = clock();
    #endif

    char * titleTypes[] = {"movie", "short", "tvMiniSeries", "tvSeries"};
    unsigned char genreDim;
    char * vecGenres[QTY_GENRES];
    loadGenres(argv[1], vecGenres, &genreDim); //TODO: Ponerlo con argv

    imdbADT imdb = newImdbADT(vecGenres, genreDim);
    csvADT csvTitles = newCsv(argv[2], READ); //TODO: Ponerlo con argv
    tTitle * title;
    size_t typesDim = sizeof(titleTypes)/sizeof(char*);
    while (!eof(csvTitles)) {
        title = readNextTitle(csvTitles, vecGenres, genreDim, titleTypes, typesDim);
        if (title != NULL) {
            loadData(imdb, title);
            freeTitle(title);
        }
    }
    closeFile(csvTitles);
    writeResults(imdb, genreDim, vecGenres);
    freeImdb(imdb);
    freeGenres(vecGenres, genreDim);

    #if DEBUG
        t = clock() - t;
        double time_taken = ((double)t)/CLOCKS_PER_SEC;
        printf("Tardo: %f\n", time_taken);
    #endif

    return 0;
}

void loadGenres(char * fileName, char * vecGenres[], unsigned char * genreDim) {
    csvADT genresCsv = newCsv(fileName, READ);
    int i = 0;
    while (i < QTY_GENRES && !eof(genresCsv))
        vecGenres[i++] = readNextString(genresCsv);
    *genreDim = i > 0 ? i - 1 : 0;
    closeFile(genresCsv);
}

void freeGenres(char ** genres, unsigned char genreDim){
    for(int i = 0; i < genreDim; i++)
        free(genres[i]);
}

void writeMoviesRec(imdbADT imdb, unsigned short year, csvADT csv) {
    if (!hasNextMovie(imdb))
        return;
    char * title = getCurrentMovieTitle(imdb);
    unsigned long votes = getCurrentMovieVotes(imdb);
    float rating = getCurrentMovieRaiting(imdb);
    char * genres = getCurrentMovieGenres(imdb);
    nextMovie(imdb);
    writeMoviesRec(imdb, year, csv);
    writeQuery3(csv, year, title, votes, rating, genres);
    free(genres);
}

void writeResults(imdbADT imdb, unsigned char genreDim, char ** genres){
    toBeginYear(imdb);
    csvADT query1File = newCsv(QUERY1_FILE, WRITE);
    csvADT query2File = newCsv(QUERY2_FILE, WRITE);
    csvADT query3File = newCsv(QUERY3_FILE, WRITE);
    writeString(query1File, QUERY1_HEADERS);
    writeString(query2File, QUERY2_HEADERS);
    writeString(query3File, QUERY3_HEADERS);
    while (hasNextYear(imdb)){
        unsigned short year = getCurrentYear(imdb);

        //Query 1
        writeQuery1(query1File, year, getQtyFilms(imdb), getQtySeries(imdb), getQtyShorts(imdb));

        //Query 2
        writeQuery2(query2File, year, genreDim, genres, getQtyByGenresByYear(imdb));

        // Query 3
        toBeginMovie(imdb);
        writeMoviesRec(imdb, year, query3File);
        nextYear(imdb);
    }
    closeFile(query1File);
    closeFile(query2File);
    closeFile(query3File);
}

