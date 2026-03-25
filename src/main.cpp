#define SDL_MAIN_USE_CALLBACKS  // Use SDL callbacks
// Window dimensions
#define WNDW_W 640
#define WNDW_H 480

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <glm/glm.hpp>

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture *texture;

glm::vec3 bg_color = { 0.2f, 0.2f, 0.2f };

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("[ERROR] - Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    const SDL_WindowFlags flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;

    if (!SDL_CreateWindowAndRenderer("LearnSDL3", WNDW_W, WNDW_H, flags, &window, &renderer)) {
        SDL_Log("[ERROR] Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, WNDW_W, WNDW_H, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, WNDW_W, WNDW_H);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    // Verteces position for the triangle
    glm::vec3 verteces[3] = {
        {-0.5f, -0.5f, 0.0f},
        {0.5f, -0.5f, 0.0f},
        { 0.0f,  0.5f, 0.0f},
    };

    SDL_SetRenderDrawColorFloat(renderer, bg_color.r, bg_color.g, bg_color.b, SDL_ALPHA_OPAQUE_FLOAT);

    SDL_RenderClear(renderer);      // Clear window
    SDL_RenderPresent(renderer);    // Update screen

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
        // Close the window on request
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        // Button input
        switch (event->key.key) {
        case SDLK_ESCAPE: // Exit with Escape key
            return SDL_APP_SUCCESS;
        case SDLK_UP:
            bg_color = glm::clamp(bg_color + glm::vec3(0.05f, 0.f, 0.f), glm::vec3(0), glm::vec3(1)); // Make background slightly more red with Up key
            break;
        case SDLK_LEFT:
            bg_color = glm::clamp(bg_color + glm::vec3(0.f, 0.05f, 0.f), glm::vec3(0), glm::vec3(1)); // Make background slightly more green with Left key
            break;
        case SDLK_RIGHT:
            bg_color = glm::clamp(bg_color + glm::vec3(0.f, 0.f, 0.05f), glm::vec3(0), glm::vec3(1)); // Make background slightly more blue with Right key
            break;
        case SDLK_DOWN:
            bg_color = glm::vec3(0.2f, 0.2f, 0.2f); // Reset background with Down key
            break;
        }
        break;
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    if (result == SDL_APP_FAILURE) {
        SDL_Log("[ERROR]: %s", SDL_GetError());
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}