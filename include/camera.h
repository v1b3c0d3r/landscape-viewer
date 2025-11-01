#ifndef CAMERA_H
#define CAMERA_H

#include <cglm/cglm.h>
#include <SDL2/SDL.h>

typedef struct {
    vec3 position;
    vec3 front;
    vec3 up;
    float yaw;
    float pitch;
    float speed;
} Camera;

Camera* camera_create(vec3 position);
void camera_get_view_matrix(Camera* camera, mat4 view);
void camera_process_keyboard(Camera* camera, const Uint8* state, float delta_time);
void camera_process_mouse(Camera* camera, float xoffset, float yoffset);

#endif
