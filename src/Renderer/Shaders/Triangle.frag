#version 450

out vec4 FragColor;
uniform vec4 InColor;
uniform float InTime;

void main()
{
    FragColor = InColor;
}