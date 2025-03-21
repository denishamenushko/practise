#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>
//#include <flecs/addons/cpp/world/hpp>
#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "Node.hpp"
#include "Texture.hpp"

class Model
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

public:
    Model();
    ~Model();

    SDL_AppResult init();
    SDL_AppResult onEvent(SDL_Event *event);
    SDL_AppResult onKeyDownEvent(SDL_KeyboardEvent &event);
    SDL_AppResult iterate();

protected:
    void clearWindow();
    void initMechanism();
    void solveMechanism();

    // void renderMechanism();
    flecs::system createRenderMechanismSystem();


    Texture createBaseTexture();

    flecs::world ecs;
    flecs:: system renderMechanism;
    flecs::entity e0;
    flecs::entity e1;

    double phi; //  угол радианы степень свободы

    double l1; //длина 1 звена, м
    double l2; //длина 2 звена, м
    glm::dvec2 p0;

    glm::dvec2 p1, p2;

};
