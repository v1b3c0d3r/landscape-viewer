#include <stdio.h>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "heightmap.h"
#include "landscape.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include <cglm/cglm.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <heightmap.png> [texture.png]\n", argv[0]);
        return 1;
    }

    HeightMap* heightmap = load_heightmap(argv[1]);
    if (!heightmap) {
        return 1;
    }

    Landscape* landscape = create_landscape(heightmap);
    if (!landscape) {
        free_heightmap(heightmap);
        return 1;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Could not initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window* window = SDL_CreateWindow(
        "3D Landscape",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        0, 0,
        SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP
    );

    if (!window) {
        fprintf(stderr, "Could not create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        fprintf(stderr, "Could not create OpenGL context: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return 1;
    }

    Texture* texture = NULL;
    if (argc > 2) {
        texture = texture_create(argv[2]);
    }

    Shader* shader = shader_create("shaders/landscape.vert", "shaders/landscape.frag");

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, landscape->vertex_count * 5 * sizeof(float), landscape->vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Camera* camera = camera_create((vec3){0.0f, 1.0f, 3.0f});

    SDL_SetRelativeMouseMode(SDL_TRUE);

    mat4 model, view, projection;
    glm_mat4_identity(model);
    glm_perspective(glm_rad(45.0f), 800.0f / 600.0f, 0.1f, 100.0f, projection);

    if (texture) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    float last_frame = 0.0f;
    int running = 1;
    while (running) {
        float current_frame = SDL_GetTicks() / 1000.0f;
        float delta_time = current_frame - last_frame;
        last_frame = current_frame;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = 0;
                }
            }
            if (event.type == SDL_MOUSEMOTION) {
                camera_process_mouse(camera, event.motion.xrel, -event.motion.yrel);
            }
        }

        const Uint8* state = SDL_GetKeyboardState(NULL);
        camera_process_keyboard(camera, state, delta_time);
        camera_get_view_matrix(camera, view);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader_use(shader);
        shader_set_mat4(shader, "model", model);
        shader_set_mat4(shader, "view", view);
        shader_set_mat4(shader, "projection", projection);

        if (texture) {
            texture_bind(texture, 0);
        }

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, landscape->vertex_count);

        SDL_GL_SwapWindow(window);
    }

    free(camera);
    if (texture) {
        texture_delete(texture);
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    shader_delete(shader);
    free_landscape(landscape);
    free_heightmap(heightmap);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
