#version 330 core

out vec4 Color;
in vec4 FragColor;

void main()
{
    Color = FragColor;
}
