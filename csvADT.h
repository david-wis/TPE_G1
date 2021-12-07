//
// Created by mbp13 on 03/12/2021.
//

#ifndef TPE_G1_CSVADT_H
#define TPE_G1_CSVADT_H
typedef struct csvCDT * csvADT;
static char * readLine(FILE * f);
int closeFile(csvADT csv);
#endif //TPE_G1_CSVADT_H
