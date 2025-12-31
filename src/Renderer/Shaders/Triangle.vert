#version 450

layout (location = 0) in vec3 aPos;

uniform mat4 inTransform;

void main()
{
    gl_Position = inTransform * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}