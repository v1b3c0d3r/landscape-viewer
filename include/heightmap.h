#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

typedef struct {
    int width;
    int height;
    unsigned char* data;
} HeightMap;

HeightMap* load_heightmap(const char* filename);
void free_heightmap(HeightMap* heightmap);

#endif
