#version 330 core
layout (location = 0) in vec3 gPos;
layout (location = 1) in vec4 gColor;

out vec4 FragColor;

void main()
{
    gl_Position = vec4(gPos, 1.0f);
    FragColor = gColor;
}
