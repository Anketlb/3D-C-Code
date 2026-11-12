#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <compile_shader_program.h>
#include <iostream>
#include <cmath>

using namespace std;

const char *vertexShaderSource = R"(#version 330 core

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
)";

const char *fragmentShaderTealSource = R"(#version 330 core

in vec3 pos;
in vec3 uv;
in vec3 normal;

out vec4 FragColor;

void main()
{
   FragColor = vec4(pos.x, pos.y, 0.0f, 1.0f);
}
)";

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "CRITICAL ERROR: Failed to initialize GLFW framework" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *window = glfwCreateWindow(800, 600, "CSCI 4620", NULL, NULL);

    if (!window)
    {
        std::cerr << "CRITICAL ERROR: Failed to create OS Window via GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "CRITICAL ERROR: GLAD failed to map system OpenGL driver pointers" << std::endl;
        return -1;
    }

    unsigned int shaderProgramTriangle = compileShaderProgram(vertexShaderSource, fragmentShaderTealSource);

    float triangleVertices[] = {
        /*position*/ -.9f, -.9f, -20.0f, /*uv*/ 0,0, /*normals*/ 0,0,1, // Vertex one
        /*position*/.9f,  -.9f, -20.0f,  /*uv*/ 0,0, /*normals*/ 0,0,1, // Vertex two
        /*position*/.9f,  .9f,  -20.0f,  /*uv*/ 0,0, /*normals*/ 0,0,1, // Vertex three

        // Tri 2
        /*position*/-.9f, -.9f, -20.0f,  /*uv*/ 0,0, /*normals*/ 0,0,1, // Vertex one
        /*position*/-.9f,  .9f, -20.0f,  /*uv*/ 0,0, /*normals*/ 0,0,1, // Vertex two
        /*position*/.9f,  .9f,  -20.0f,  /*uv*/ 0,0, /*normals*/ 0,0,1, // Vertex three
    };

    unsigned int VAO_Triangle, VBO_Triangle;
    glGenVertexArrays(1, &VAO_Triangle);
    glGenBuffers(1, &VBO_Triangle);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Triangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO_Triangle);
    //positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    //uvs
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    //normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(5*sizeof(float)));
    glEnableVertexAttribArray(2);

    int modelToWorldPointer = glGetUniformLocation(shaderProgramTriangle, "modelToWorld");
    int worldToCameraPointer = glGetUniformLocation(shaderProgramTriangle, "worldToCamera");
    int ProjectionPointer = glGetUniformLocation(shaderProgramTriangle, "projection");
    int toClippingSpacePointer = glGetUniformLocation(shaderProgramTriangle, "toClippingSpace");

    float nearPlane = .1f;
    float farPlane = 100.0f;

    float FOV = 45;
    float radians = FOV/360 * 2 * 3.14159f;
    float aspectRatio = 800/600.0f;
    float top = nearPlane * tan(radians/2);
    float right = top * aspectRatio;

    float modelToWorld[] = {
        1, 0, 0, 0, //
        0, 1, 0, 0, //
        0, 0, 1, 0, //
        0, 0, 0, 1, //
    };
    float worldToCamera[] = {
        1, 0, 0, 0, //
        0, 1, 0, 0, //
        0, 0, 1, 0, //
        0, 0, 0, 1, //
    };
    float projection[] = {
        1, 0, 0, 0, //
        0, 1, 0, 0, //
        0, 0, (nearPlane+farPlane), nearPlane*farPlane, //
        0, 0, -1, 0, //
    };
    float toClippingSpace[] = {
        nearPlane/right, 0, 0, 0, //
        0, nearPlane/top, 0, 0, //
        0, 0, -2/(farPlane-nearPlane), -(farPlane+nearPlane)/(farPlane-nearPlane), //
        0, 0, 0, 1, //
    };

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgramTriangle);

        glUniformMatrix4fv(modelToWorldPointer,1,GL_TRUE, modelToWorld);
        glUniformMatrix4fv(worldToCameraPointer,1,GL_TRUE, worldToCamera);
        glUniformMatrix4fv(ProjectionPointer,1,GL_TRUE, projection);
        glUniformMatrix4fv(toClippingSpacePointer,1,GL_TRUE, toClippingSpace);

        glBindVertexArray(VAO_Triangle);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO_Triangle);
    glDeleteBuffers(1, &VBO_Triangle);
    glDeleteProgram(shaderProgramTriangle);

    glfwTerminate();
    return 0;
}