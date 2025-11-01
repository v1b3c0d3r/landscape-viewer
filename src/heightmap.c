#include "heightmap.h"
#include <stdio.h>
#include <stdlib.h>

#include "stb_image.h"

HeightMap* load_heightmap(const char* filename) {
    HeightMap* heightmap = (HeightMap*)malloc(sizeof(HeightMap));
    if (!heightmap) {
        fprintf(stderr, "Could not allocate memory for HeightMap\n");
        return NULL;
    }

    int channels;
    heightmap->data = stbi_load(filename, &heightmap->width, &heightmap->height, &channels, 1);
    if (!heightmap->data) {
        fprintf(stderr, "Could not load heightmap from %s: %s\n", filename, stbi_failure_reason());
        free(heightmap);
        return NULL;
    }

    return heightmap;
}

void free_heightmap(HeightMap* heightmap) {
    if (heightmap) {
        stbi_image_free(heightmap->data);
        free(heightmap);
    }
}
