//
// Created by david on 12/3/21.
//

#include "title.h"
#include <stdlib.h>

void freeTitle(tTitle * t) {
    if (t != NULL) {
        free(t->id);
        free(t->primaryTitle);
    }
    free(t);
}
