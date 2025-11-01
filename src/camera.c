#include "camera.h"
#include <SDL2/SDL.h>

Camera* camera_create(vec3 position) {
    Camera* camera = (Camera*)malloc(sizeof(Camera));
    glm_vec3_copy(position, camera->position);
    glm_vec3_copy((vec3){0.0f, 0.0f, -1.0f}, camera->front);
    glm_vec3_copy((vec3){0.0f, 1.0f, 0.0f}, camera->up);
    camera->yaw = -90.0f;
    camera->pitch = 0.0f;
    camera->speed = 2.5f;
    return camera;
}

void camera_get_view_matrix(Camera* camera, mat4 view) {
    vec3 center;
    glm_vec3_add(camera->position, camera->front, center);
    glm_lookat(camera->position, center, camera->up, view);
}

void camera_process_keyboard(Camera* camera, const Uint8* state, float delta_time) {
    float velocity = camera->speed * delta_time;
    if (state[SDL_SCANCODE_W]) {
        vec3 scaled_front;
        glm_vec3_scale(camera->front, velocity, scaled_front);
        glm_vec3_add(camera->position, scaled_front, camera->position);
    }
    if (state[SDL_SCANCODE_S]) {
        vec3 scaled_front;
        glm_vec3_scale(camera->front, velocity, scaled_front);
        glm_vec3_sub(camera->position, scaled_front, camera->position);
    }
    if (state[SDL_SCANCODE_A]) {
        vec3 cross_product, scaled_cross;
        glm_vec3_cross(camera->front, camera->up, cross_product);
        glm_normalize(cross_product);
        glm_vec3_scale(cross_product, velocity, scaled_cross);
        glm_vec3_sub(camera->position, scaled_cross, camera->position);
    }
    if (state[SDL_SCANCODE_D]) {
        vec3 cross_product, scaled_cross;
        glm_vec3_cross(camera->front, camera->up, cross_product);
        glm_normalize(cross_product);
        glm_vec3_scale(cross_product, velocity, scaled_cross);
        glm_vec3_add(camera->position, scaled_cross, camera->position);
    }
}

void camera_process_mouse(Camera* camera, float xoffset, float yoffset) {
    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera->yaw += xoffset;
    camera->pitch += yoffset;

    if (camera->pitch > 89.0f) {
        camera->pitch = 89.0f;
    }
    if (camera->pitch < -89.0f) {
        camera->pitch = -89.0f;
    }

    vec3 front;
    front[0] = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    front[1] = sin(glm_rad(camera->pitch));
    front[2] = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    glm_normalize(front);
    glm_vec3_copy(front, camera->front);
}
