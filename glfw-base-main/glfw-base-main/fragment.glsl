#version 330 core

in vec3 pos;
in vec3 uv;
in vec3 normal;

out vec4 FragColor;

void main()
{
   FragColor = vec4(pos.x, pos.y, 0.0f, 1.0f);
}