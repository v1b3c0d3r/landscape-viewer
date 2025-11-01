#ifndef SHADER_H
#define SHADER_H

#include <cglm/cglm.h>

typedef struct {
    unsigned int id;
} Shader;

Shader* shader_create(const char* vertex_path, const char* fragment_path);
void shader_use(Shader* shader);
void shader_set_mat4(Shader* shader, const char* name, mat4 mat);
void shader_delete(Shader* shader);

#endif
