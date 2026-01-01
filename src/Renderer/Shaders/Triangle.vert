#version 450

layout (location = 0) in vec3 InPosition;
layout (location = 1) in vec4 InColor;

layout (location = 0) out vec4 OutColor;

uniform mat4 InMVP;

void main()
{
    gl_Position = InMVP * vec4(InPosition.x, InPosition.y, InPosition.z, 1.0);
    OutColor = InColor;
}