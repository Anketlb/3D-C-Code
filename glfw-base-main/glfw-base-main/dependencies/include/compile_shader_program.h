#ifndef COMPILE_SHADER_PROGRAM_H
#define COMPILE_SHADER_PROGRAM_H

#include <glad/glad.h> // Declares the gl* functions and GL_* constants used below
#include <iostream>

// Prints the driver's error message if the shader failed to compile
static void checkShaderCompile(unsigned int shader, const char *name)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char log[1024];
        glGetShaderInfoLog(shader, sizeof(log), NULL, log);
        std::cerr << "SHADER COMPILE ERROR (" << name << "):\n" << log << std::endl;
    }
}

// Prints the driver's error message if the program failed to link
static void checkProgramLink(unsigned int program)
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char log[1024];
        glGetProgramInfoLog(program, sizeof(log), NULL, log);
        std::cerr << "SHADER LINK ERROR:\n" << log << std::endl;
    }
}

static unsigned int compileShaderProgram(const char *vertSrc, const char *fragSrc)
{
    // Create space for a new shader on the GPU and return a handle to the new shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Assign the actual code for the shader to the newly created shader on the gpu
    glShaderSource(vertexShader, 1, &vertSrc, NULL);
    // Compile the shader code
    glCompileShader(vertexShader);
    checkShaderCompile(vertexShader, "vertex");

    // Create space for a new shader on the GPU and return a handle to the new shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Assign the actual code for the fragment shader
    glShaderSource(fragmentShader, 1, &fragSrc, NULL);
    // Compile the fragment shader code
    glCompileShader(fragmentShader);
    checkShaderCompile(fragmentShader, "fragment");

    // Create a program on the GPU.
    // This will allow us to render triangles on the GPU
    unsigned int program = glCreateProgram();
    // Tell the program about the vertex shader
    glAttachShader(program, vertexShader);
    // Tell the program about the fragment shader
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    checkProgramLink(program);

    // House keeping--we're telling the GPU that we don't need them by themselves anymore
    // Since the program still has pointers to it, the shaders actually won't be removed until
    // the program itself is removed.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Return the program so we can call it every frame
    return program;
}

#endif