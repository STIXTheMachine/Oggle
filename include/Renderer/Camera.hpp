//
// Created by jake on 1/1/26.
//

#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

namespace Oggle
{
struct Camera
{
    explicit Camera(glm::vec3 InitialPosition);

    void Translate(glm::vec3 Translation);
    void TranslateLocal(glm::vec3 Translation);
    void PitchUp(float PitchDelta_Degrees);
    void YawRight(float YawDelta_Degrees);

    glm::mat4 GetViewMatrix() const;
    glm::mat3 GetRotationMatrix() const;

    glm::vec3 GetForwardVector() const;
    glm::vec3 GetRightVector() const;
    glm::vec3 GetUpVector() const;

    float VerticalFOV { 30.f };
private:
    static inline glm::vec3 WorldForward { 0.f, 0.f, 1.f };
    static inline glm::vec3 WorldUp      { 0.f, 1.f, 0.f };
    static inline glm::vec3 WorldRight   { 1.f, 0.f, 0.f };

    float Pitch = 0.f;
    float Yaw = 0.f;

    glm::vec3 Position;
};
}
