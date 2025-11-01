#include "texture.h"
#include "stb_image.h"
#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

Texture* texture_create(const char* filename) {
    Texture* texture = (Texture*)malloc(sizeof(Texture));
    if (!texture) {
        fprintf(stderr, "Could not allocate memory for Texture\n");
        return NULL;
    }

    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) {
        fprintf(stderr, "Could not load texture from %s: %s\n", filename, stbi_failure_reason());
        free(texture);
        return NULL;
    }

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    GLenum format;
    if (channels == 1) {
        format = GL_RED;
    } else if (channels == 3) {
        format = GL_RGB;
    } else if (channels == 4) {
        format = GL_RGBA;
    } else {
        fprintf(stderr, "Unsupported number of channels: %d\n", channels);
        stbi_image_free(data);
        free(texture);
        return NULL;
    }

    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texture;
}

void texture_bind(Texture* texture, unsigned int unit) {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture->id);
}

void texture_delete(Texture* texture) {
    if (texture) {
        glDeleteTextures(1, &texture->id);
        free(texture);
    }
}
