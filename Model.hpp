#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>

#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <array>

#include "Node.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

class Model
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

public:
    Model();
    ~Model();

    SDL_AppResult init();
    SDL_AppResult onEvent(SDL_Event &event);
    SDL_AppResult onKeyDownEvent(SDL_KeyboardEvent &event);
    SDL_AppResult iterate();

protected:
    void clearWindow();

    void initMechanism();
    void solveMechanism();
    void updateCamera();
    void updateNodes();

    flecs::system createRenderMechaismSystem();

    Texture createBaseTexture(double w, double h);
    Texture createLinkTexture(double l);

    flecs::world ecs;
    flecs::system renderMechanism;
    flecs::entity e0, e1, e2, e3, e4, e5, e6, e7;

    double scale = 100.0;

    Camera camera;

    double phi; // Угол, рад

    glm::dvec2 p0; // База

    double l1; // Длина звена 1, м
    double a1; // Угол поворота звена 1, рад

    double l2; // Длина звена2, м
    double a2; // Угол поворота звена 2, рад

    double l3; // Длина звена3, м
    double a3; // Угол поворота звена , рад

    double l4; // Длина звена4, м
    double a4; // Угол поворота звена , рад

    double l5; // Длина звена, м
    double a5; // Угол поворота звена , рад


    glm::dvec2 p1, p2, p3, p4, p5, p6;

    bool showBase = true;
};
