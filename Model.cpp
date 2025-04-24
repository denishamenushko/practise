#include "Model.hpp"


Model::Model()
{
    SDL_Log("[Model::Model] %p", this);
}

Model::~Model()
{
    SDL_Log("[Model::~Model] %p", this);
}

SDL_AppResult Model::init()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't init SDL: %s"
                , SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(
            "Model"
            , 800, 800
            , SDL_WINDOW_RESIZABLE
            , &this->window
            , &this->renderer
            )
        )
    {
        SDL_Log("Couldn't create window/renderer: %s"
                , SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Инициализация механизма
    this->initMechanism();

    return SDL_APP_CONTINUE;
}

SDL_AppResult Model::onEvent(SDL_Event &event)
{
    switch (event.type) {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
    case SDL_EVENT_KEY_DOWN:
        return this->onKeyDownEvent(event.key);
    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult Model::onKeyDownEvent(SDL_KeyboardEvent &event)
{
    switch (event.key) {
    case SDLK_B:
        this->showBase = !this->showBase;
        if (this->showBase)
        {
            this->e0.enable();
            this->e3.enable();
        } else {
            this->e0.disable();
            this->e3.disable();
        }
        break;
    case SDLK_SPACE:
        this->phi += glm::radians(5.0);
        break;
    default:
        break;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult Model::iterate()
{
    this->clearWindow();

    this->solveMechanism();

    this->updateCamera();

    this->updateNodes();

    this->ecs.progress();

    SDL_RenderPresent(this->renderer);

    return SDL_APP_CONTINUE;
}

void Model::clearWindow()
{
    // Задаем цвет фона
    SDL_SetRenderDrawColorFloat(
        this->renderer
        , 0.5f, 0.5f, 0.5f
        , SDL_ALPHA_OPAQUE_FLOAT);
    // Очищаем экран
    SDL_RenderClear(this->renderer);
}

void Model::initMechanism()
{
    this->phi = 1.57;

    this->p0 = {0.0, 0.0};
    this->p2 = {2.0, 0.0};
    this->p5 = {1.4, 2.6};
    this->l1 = 1.0;
    this->l2 = 2.0;
    this->l3 = 2.0;
    this->l4 = 2.0;
    this->l5 = 5.0;
    this->renderMechanism = this->createRenderMechaismSystem();

    this->e0 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createBaseTexture(0.4, 0.2);
        });

    this->e1 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createLinkTexture(this->l1);
        });

    this->e2 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createLinkTexture(this->l2);
        });

    this->e3 = this->ecs.entity()
        .insert([this](Texture &t)
        {
            t = this->createBaseTexture(0.4, 0.2);
        });

    this->e4 = this->ecs.entity()
                   .insert([this](Texture &t)
                           {
                               t = this->createLinkTexture(this->l3);
                           });
    this->e5 = this->ecs.entity()
                   .insert([this](Texture &t)
                           {
                               t = this->createBaseTexture(0.4, 0.2);
                           });
    this->e6 = this->ecs.entity()
                   .insert([this](Texture &t)
                           {
                               t = this->createLinkTexture(this->l4);
                           });
    this->e7 = this->ecs.entity()
                   .insert([this](Texture &t)
                           {
                               t = this->createLinkTexture(this->l5);
                           });
}

void Model::solveMechanism()
{
    this->a1 = phi;

    this->p1 = {this->l1, 0.0};
    this->p1 = glm::rotate(this->p1, this->a1);
    this->p1 = this->p0 + this->p1;

    this->a2 =glm::atan2(-glm::sin(this->a1),2-glm::cos(this->a1))
               +glm::acos((1+4+4-4-2*2*glm::cos(this->a1))/(2*2*sqrt(1+4-2*2*glm::cos(this->a1))));
    this->a3=glm::atan2(this->p2.y-this->p3.y,this->p2.x-this->p3.x);
    this->p3 = {this->l2, 0.0};
    this->p3 = glm::rotate(this->p3, this->a2);
    this->p3 = this->p1 + this->p3;//это точка B на моей схеме

    this->a4 = (this->a2)+1.57;
    this->p6 = {this->l2, 0.0};
    this->p6 = glm::rotate(this->p6, this->a4);
    this->p6 = this->p1 + this->p6;//точка Е

    this->a5 = glm::atan(this->p6.y-this->p5.y,this->p6.x-this->p5.x);



}

void Model::updateNodes()
{
    this->e0.set<Node>(this->camera.toRendererNode(
        this->p0
    ));
    this->e1.set<Node>(this->camera.toRendererNode(
        this->p0,
        this->a1
    ));
    this->e2.set<Node>(this->camera.toRendererNode(
        this->p1,
        this->a2
    ));
    this->e3.set<Node>(this->camera.toRendererNode(
        this->p2
    ));
    this->e4.set<Node>(this->camera.toRendererNode(
        this->p3,
        this->a3
        ));
    this->e5.set<Node>(this->camera.toRendererNode(
        this->p5
        ));
    this->e6.set<Node>(this->camera.toRendererNode(
        this->p1,
        this->a4
        ));
    this->e7.set<Node>(this->camera.toRendererNode(
        this->p5,
        this->a5
        ));
}

void Model::updateCamera()
{
    int w, h;
    if (!SDL_GetWindowSize(this->window, &w, &h))
    {
        SDL_Log("Couldn't get window size: %s",
                SDL_GetError());
        return;
    }
    SDL_FRect rect = {
        0.0f, 0.0f,
        static_cast<float>(w),
        static_cast<float>(h),
    };
    this->camera.setRendererRect(rect);
    glm::dvec2 pos = {-4.0, -2.0};
    glm::dvec2 size = {
        static_cast<double>(w) / this->scale,
        static_cast<double>(h) / this->scale
    };
    this->camera.setSceneRect(pos, size);
}

flecs::system Model::createRenderMechaismSystem()
{
    return this->ecs.system<const Node, const Texture>()
        .each([this](const Node &n, const Texture &t)
        {
            SDL_FRect r = {
                n.position.x - t.center.x
                , n.position.y - t.center.y
                , t.rect.w, t.rect.h};
            if (n.angle)
            {
              SDL_RenderTextureRotated(
                  this->renderer
                  , t.texture
                  , &t.rect
                  , &r
                  , n.angle
                  , &t.center
                  , SDL_FLIP_NONE
                  );
            }
            else
            {
              SDL_RenderTexture(
                  this->renderer
                  , t.texture
                  , &t.rect
                  , &r
                  );
            }
        });
}

Texture Model::createBaseTexture(double w, double h)
{
    Texture result;

    Camera camera;
    glm::dvec2 pos = {-w / 2.0 - 1.0, -h - 1.0};
    glm::dvec2 size = {w + 2.0, h + 2.0};
    camera.setSceneRect(pos, size);

    result.center = {
        static_cast<float>(-pos.x * this->scale),
        static_cast<float>(-pos.y * this->scale)
    };
    result.rect = {
        0.0f, 0.0f,
        static_cast<float>(size.x * this->scale),
        static_cast<float>(size.y * this->scale)
    };
    camera.setRendererRect(result.rect);

    SDL_Surface *surface = SDL_CreateSurface(
        static_cast<int>(result.rect.w) // Ширина
        , static_cast<int>(result.rect.h) // Высота
        , SDL_PIXELFORMAT_RGBA32
        );
    SDL_Renderer *renderer =
        SDL_CreateSoftwareRenderer(surface);
    SDL_FPoint base[4];
    result.center = base[0] = base[3] =
        camera.toRenderer({0.0, 0.0});
    base[1] = camera.toRenderer({-w / 2.0f, -h});
    base[2] = camera.toRenderer({w / 2.0f, -h});
    SDL_SetRenderDrawColorFloat(
        renderer
        , 0.0f, 0.0f, 0.0f
        , SDL_ALPHA_OPAQUE_FLOAT
        );
    SDL_RenderLines(
        renderer
        , base
        , SDL_arraysize(base)
        );
    SDL_RenderPresent(renderer);
    result.texture =
        SDL_CreateTextureFromSurface(
            this->renderer
            , surface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroySurface(surface);
    return result;
}

Texture Model::createLinkTexture(double l)
{
    Texture result;

    Camera camera;
    glm::dvec2 pos = {-1.0, -1.0};
    glm::dvec2 size = {l + 2.0, 2.0};
    camera.setSceneRect(pos, size);

    result.center = {
        static_cast<float>(-pos.x * this->scale),
        static_cast<float>(-pos.y * this->scale)
    };
    result.rect = {
        0.0f, 0.0f,
        static_cast<float>(size.x * this->scale),
        static_cast<float>(size.y * this->scale)
    };
    camera.setRendererRect(result.rect);

    std::array<SDL_FPoint, 2> link;
    link[0] = camera.toRenderer({0.0, 0.0});
    link[1] = camera.toRenderer({l, 0.0});

    SDL_Surface *surface = SDL_CreateSurface(
        static_cast<int>(result.rect.w) // Ширина
        , static_cast<int>(result.rect.h) // Высота
        , SDL_PIXELFORMAT_RGBA32
        );
    SDL_Renderer *renderer =
        SDL_CreateSoftwareRenderer(surface);

    SDL_SetRenderDrawColorFloat(
        renderer
        , 0.0f, 0.0f, 0.0f
        , SDL_ALPHA_OPAQUE_FLOAT
        );
    SDL_RenderLines(
        renderer
        , link.data()
        , link.size()
        );
    SDL_RenderPresent(renderer);

    result.texture =
        SDL_CreateTextureFromSurface(
            this->renderer
            , surface);

    SDL_DestroyRenderer(renderer);
    SDL_DestroySurface(surface);

    return result;
}
