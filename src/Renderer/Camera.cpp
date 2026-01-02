//
// Created by jake on 1/1/26.
//

#include "Renderer/Camera.hpp"
#include "Core/Logging/Logging.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Oggle
{
Camera::Camera(glm::vec3 InitialPosition)
    : Position(InitialPosition)
{}

void Camera::Translate(glm::vec3 Translation)
{
    Position += Translation;
}

void Camera::TranslateLocal(glm::vec3 Translation)
{
    Position += GetRotationMatrix() * Translation;
}

void Camera::PitchUp(float PitchDelta_Degrees)
{
    PitchDelta_Degrees  =
        glm::clamp(
            PitchDelta_Degrees,
            -89.f - Pitch,
            89.f - Pitch
        );

    Pitch += PitchDelta_Degrees;
}

void Camera::YawRight(float YawDelta_Degrees)
{
    Yaw += YawDelta_Degrees;
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + GetForwardVector(), WorldUp);
}

glm::mat3 Camera::GetRotationMatrix() const
{
    return glm::mat3 { GetRightVector(), GetUpVector(), GetForwardVector() };
}

glm::vec3 Camera::GetForwardVector() const
{
    const auto YawedForward = glm::angleAxis(glm::radians(Yaw), WorldUp) * WorldForward;
    const auto TrueForward = glm::angleAxis(glm::radians(Pitch), GetRightVector()) * YawedForward;
    return TrueForward;
}

glm::vec3 Camera::GetRightVector() const
{
    return glm::angleAxis(glm::radians(Yaw), WorldUp) * WorldRight;
}

glm::vec3 Camera::GetUpVector() const
{
    return glm::normalize(glm::cross(GetForwardVector(), GetRightVector()));
}
}
