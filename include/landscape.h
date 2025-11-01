#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include "heightmap.h"

typedef struct {
    float* vertices;
    int vertex_count;
} Landscape;

Landscape* create_landscape(HeightMap* heightmap);
void free_landscape(Landscape* landscape);

#endif
