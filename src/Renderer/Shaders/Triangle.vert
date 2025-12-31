#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

layout (location = 0) out vec4 vColor;

uniform mat4 inTransform;

void main()
{
    gl_Position = inTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vColor = aColor;
}