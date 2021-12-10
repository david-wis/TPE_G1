//
// Created by david on 12/3/21.
//

#ifndef TPE_G1_TITLE_H
#define TPE_G1_TITLE_H

typedef enum { MOVIE = 0, SHORT, TVMINISERIES, TVSERIES } tTitleType;

//estructura de titulo
typedef struct tTitle{
    char * id;
    char * primaryTitle;
    tTitleType titleType;
    unsigned int genres; // Se usa cada bit para un genero
    unsigned int runtimeMinutes;
    unsigned short startYear;
    unsigned short endYear;
    unsigned long numVotes;
    float avgRating;
} tTitle;

void freeTitle(tTitle * t);

#endif //TPE_G1_TITLE_H
