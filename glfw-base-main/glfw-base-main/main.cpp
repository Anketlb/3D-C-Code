#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <compile_shader_program.h>
#include <iostream>
#include <cmath>
#include "file_util.h"
#include "triangleVertices.h"
#include "roomVertices.h"

using namespace std;

const char *vertexShaderSource = load_file("vertex.glsl");

const char *fragmentShaderTealSource = load_file("fragment.glsl");

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

    //first obj
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
    
    //second obj
    unsigned int VAO_Triangle_room, VBO_Triangle_room;
    glGenVertexArrays(1, &VAO_Triangle_room);
    glGenBuffers(1, &VBO_Triangle_room);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_Triangle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(roomVertices), roomVertices, GL_STATIC_DRAW);
    glBindVertexArray(VAO_Triangle_room);
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
        0, 0, 1, -20, //
        0, 0, 0, 1, //
    };

    float modelToWorld_room[] = {
        30, 0, 0, 0, //
        0, 1, 0, 0, //
        0, 0, 30, 0, //
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


    glEnable(GL_DEPTH_TEST);

    float camera_right_x = 1;
    float camera_right_z = 0;

    float camera_forward_x = 0;
    float camera_forward_z = -1;

    float camera_x = 0;
    float camera_z = 0;

    float speed = .01f;
    float rotation = 0;

    double mouse_x = 0;
    double mouse_y = 0;

    bool last_hover = false;

    while (!glfwWindowShouldClose(window))
    {

        if(glfwGetWindowAttrib(window, GLFW_HOVERED)){
            double x;
            double y;
            glfwGetCursorPos(window, &x, &y);
            if(last_hover){
                //rotation
                double delta_x = x - mouse_x;
                double delta_y = y - mouse_y;

                rotation += delta_x * .01f;
            }
            else{

            }
            last_hover = true;
            mouse_x = x;
            mouse_y = y;
        }
        else{
            last_hover = false;
        }

        float c = cos(rotation);
        float s = sin(rotation);

        camera_right_x = c;
        camera_right_z = -s;

        camera_forward_x = -s;
        camera_forward_z = -c;

        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
            camera_x -= camera_right_x * speed;
            camera_z -= camera_right_z * speed;
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
             camera_x += camera_right_x * speed;
             camera_z += camera_right_z * speed;
        }
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
             camera_x += camera_forward_x* speed;
             camera_z += camera_forward_z* speed;
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
            camera_x -= camera_forward_x* speed;
            camera_z -= camera_forward_z* speed;
        }

        worldToCamera[3] = -(c*camera_x-s*camera_z);
        worldToCamera[11] = -(s*camera_x-c*camera_z);
        
        worldToCamera[0] = c;
        worldToCamera[2] = -s;

        worldToCamera[8] = s;
        worldToCamera[10] = c;

        glClearColor(0.f, 0.f, 0.f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //first obj
        glUseProgram(shaderProgramTriangle);

        glUniformMatrix4fv(modelToWorldPointer,1,GL_TRUE, modelToWorld);
        glUniformMatrix4fv(worldToCameraPointer,1,GL_TRUE, worldToCamera);
        glUniformMatrix4fv(ProjectionPointer,1,GL_TRUE, projection);
        glUniformMatrix4fv(toClippingSpacePointer,1,GL_TRUE, toClippingSpace);

        glBindVertexArray(VAO_Triangle);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        //second obj
        glUseProgram(shaderProgramTriangle);

        glUniformMatrix4fv(modelToWorldPointer,1,GL_TRUE, modelToWorld_room);
        glUniformMatrix4fv(worldToCameraPointer,1,GL_TRUE, worldToCamera);
        glUniformMatrix4fv(ProjectionPointer,1,GL_TRUE, projection);
        glUniformMatrix4fv(toClippingSpacePointer,1,GL_TRUE, toClippingSpace);

        glBindVertexArray(VAO_Triangle);
        glDrawArrays(GL_TRIANGLES, 0, 24);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO_Triangle);
    glDeleteBuffers(1, &VBO_Triangle);
    glDeleteProgram(shaderProgramTriangle);

    glfwTerminate();
    return 0;
}