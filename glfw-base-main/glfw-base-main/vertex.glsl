#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;
layout (location = 2) in vec3 aNormal;

uniform mat4 modelToWorld; // model
uniform mat4 worldToCamera; // view
uniform mat4 projection;
uniform mat4 toClippingSpace; // clip

out vec3 pos;
out vec2 uv;
out vec3 normal;


void main()
{
   gl_Position = toClippingSpace * projection * worldToCamera * modelToWorld * vec4(aPos, 1.0);
   pos = aPos;
   uv = aUV;
   normal = aNormal;
}
