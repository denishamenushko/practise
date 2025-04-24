#define GLM_ENABLE_EXPERIMENTAL 1
#include <glm/gtx/matrix_transform_2d.hpp>

#include "Camera.hpp"

Camera::Camera()
    : rect({0.0f, 0.0f, 1.0f, 1.0f})
    , pos({0.0, 0.0})
    , size({1.0, 1.0})
    , transform({1.0})
{}

void Camera::setRendererRect(const SDL_FRect &rect)
{
    this->rect = rect;
    this->updateTransformMatrix();
}

void Camera::setSceneRect(const glm::dvec2 &pos, const glm::dvec2 &size)
{
    this->pos = pos;
    this->size = size;
    this->updateTransformMatrix();
}

SDL_FPoint Camera::toRenderer(const glm::dvec2 &pos) const
{
    glm::dvec3 p = {pos.x, pos.y, 1.0};
    p = this->transform * p;
    return {static_cast<float>(p.x)
            , static_cast<float>(p.y)};
}

Node Camera::toRendererNode(const glm::dvec2 &pos, double angle) const
{
    return  {
        this->toRenderer(pos),
        -glm::degrees(angle)
    };
}

void Camera::updateTransformMatrix()
{
    glm::dmat3 m = {1.0};
    m = glm::translate(m, glm::dvec2{
        static_cast<double>(this->rect.x),
        static_cast<double>(this->rect.y + this->rect.h),
    });
    m = glm::scale(m, glm::dvec2{
        static_cast<double>(this->rect.w) / this->size.x,
        -static_cast<double>(this->rect.h) / this->size.y,
    });
    m = glm::translate(m, -this->pos);
    this->transform = m;
}
