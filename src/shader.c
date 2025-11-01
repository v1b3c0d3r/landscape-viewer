#include "shader.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>

char* read_file(const char* path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* buffer = (char*)malloc(length + 1);
    fread(buffer, 1, length, file);
    fclose(file);
    buffer[length] = '\0';
    return buffer;
}

Shader* shader_create(const char* vertex_path, const char* fragment_path) {
    Shader* shader = (Shader*)malloc(sizeof(Shader));

    char* vertex_source = read_file(vertex_path);
    char* fragment_source = read_file(fragment_path);

    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const char**)&vertex_source, NULL);
    glCompileShader(vertex_shader);

    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", info_log);
    }

    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const char**)&fragment_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", info_log);
    }

    shader->id = glCreateProgram();
    glAttachShader(shader->id, vertex_shader);
    glAttachShader(shader->id, fragment_shader);
    glLinkProgram(shader->id);

    glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader->id, 512, NULL, info_log);
        fprintf(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info_log);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    free(vertex_source);
    free(fragment_source);

    return shader;
}

void shader_use(Shader* shader) {
    glUseProgram(shader->id);
}

void shader_set_mat4(Shader* shader, const char* name, mat4 mat) {
    glUniformMatrix4fv(glGetUniformLocation(shader->id, name), 1, GL_FALSE, (float*)mat);
}

void shader_delete(Shader* shader) {
    glDeleteProgram(shader->id);
    free(shader);
}
