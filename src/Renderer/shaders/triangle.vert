#version 450

layout (location = 0) in vec3 aPos;

uniform mat4 ModelViewProjection;

void main()
{
    gl_position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}