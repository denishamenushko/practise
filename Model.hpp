#pragma once

#include <SDL3/SDL.h>
#include <flecs.h>
//#include <flecs/addons/cpp/world/hpp>
#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/rotate_vector.hpp>

//#include <array>

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
    SDL_AppResult onEvent(SDL_Event *event);
    SDL_AppResult onKeyDownEvent(SDL_KeyboardEvent &event);
    SDL_AppResult iterate();

protected:
    void clearWindow();
    void initMechanism();
    void solveMechanism();
    void updateCamera();
    void updateNodes();

    // void renderMechanism();
    flecs::system createRenderMechanismSystem();


    Texture createBaseTexture(double w, double h);
    Texture createLinkTexture(double l);

    flecs::world ecs;
    flecs:: system renderMechanism;
    flecs::entity e0;
    flecs::entity e1,e2, e3;

    double scale = 100.0;//коэф масштабирования

    Camera camera;

    double phi; //  угол радианы степень свободы

    double l1; //длина 1 звена, м
    double a1; // угол поворота звена 1 рад
    double l2; //длина 2 звена, м
    double a2; // угол поворота звена 2 рад

    glm::dvec2 p0;// база

    glm::dvec2 p1, p2;

    bool showBase = true;

};
