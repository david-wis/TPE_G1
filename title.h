//
// Created by david on 12/3/21.
//

#ifndef TPE_G1_TITLE_H
#define TPE_G1_TITLE_H

//estructura de titulo
typedef struct tTitle{
    char * id;
    char titleType;
    char * primaryTitle;
    char * genres; //(0 a 31 para el genero) Vector dinamico de generos -1 terminated
    unsigned short startYear;
    unsigned short endYear;
    unsigned int runtimeMinutes;
    unsigned long numVotes;
    float avgRating;
} tTitle;

#endif //TPE_G1_TITLE_H
