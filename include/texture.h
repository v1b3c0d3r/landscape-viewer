#ifndef TEXTURE_H
#define TEXTURE_H

typedef struct {
    unsigned int id;
} Texture;

Texture* texture_create(const char* filename);
void texture_bind(Texture* texture, unsigned int unit);
void texture_delete(Texture* texture);

#endif
