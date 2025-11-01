#include "landscape.h"
#include <stdlib.h>

Landscape* create_landscape(HeightMap* heightmap) {
    Landscape* landscape = (Landscape*)malloc(sizeof(Landscape));
    if (!landscape) {
        return NULL;
    }

    int width = heightmap->width;
    int height = heightmap->height;
    int num_triangles = (width - 1) * (height - 1) * 2;
    landscape->vertex_count = num_triangles * 3;
    landscape->vertices = (float*)malloc(landscape->vertex_count * 5 * sizeof(float));
    if (!landscape->vertices) {
        free(landscape);
        return NULL;
    }

    int i = 0;
    for (int y = 0; y < height - 1; ++y) {
        for (int x = 0; x < width - 1; ++x) {
            float h1 = (float)heightmap->data[(y * width) + x] / 255.0f;
            float h2 = (float)heightmap->data[(y * width) + (x + 1)] / 255.0f;
            float h3 = (float)heightmap->data[((y + 1) * width) + x] / 255.0f;
            float h4 = (float)heightmap->data[((y + 1) * width) + (x + 1)] / 255.0f;

            // Triangle 1
            landscape->vertices[i++] = (float)x;
            landscape->vertices[i++] = h1;
            landscape->vertices[i++] = (float)y;
            landscape->vertices[i++] = (float)x / (width - 1);
            landscape->vertices[i++] = (float)y / (height - 1);

            landscape->vertices[i++] = (float)(x + 1);
            landscape->vertices[i++] = h2;
            landscape->vertices[i++] = (float)y;
            landscape->vertices[i++] = (float)(x + 1) / (width - 1);
            landscape->vertices[i++] = (float)y / (height - 1);

            landscape->vertices[i++] = (float)x;
            landscape->vertices[i++] = h3;
            landscape->vertices[i++] = (float)(y + 1);
            landscape->vertices[i++] = (float)x / (width - 1);
            landscape->vertices[i++] = (float)(y + 1) / (height - 1);

            // Triangle 2
            landscape->vertices[i++] = (float)(x + 1);
            landscape->vertices[i++] = h2;
            landscape->vertices[i++] = (float)y;
            landscape->vertices[i++] = (float)(x + 1) / (width - 1);
            landscape->vertices[i++] = (float)y / (height - 1);

            landscape->vertices[i++] = (float)(x + 1);
            landscape->vertices[i++] = h4;
            landscape->vertices[i++] = (float)(y + 1);
            landscape->vertices[i++] = (float)(x + 1) / (width - 1);
            landscape->vertices[i++] = (float)(y + 1) / (height - 1);

            landscape->vertices[i++] = (float)x;
            landscape->vertices[i++] = h3;
            landscape->vertices[i++] = (float)(y + 1);
            landscape->vertices[i++] = (float)x / (width - 1);
            landscape->vertices[i++] = (float)(y + 1) / (height - 1);
        }
    }

    return landscape;
}

void free_landscape(Landscape* landscape) {
    if (landscape) {
        free(landscape->vertices);
        free(landscape);
    }
}
