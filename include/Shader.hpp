#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glad/glad.h>
#include <glm/glm.hpp>

// Loads a shader file.
std::string LoadShaderAsString(const std::string &fileName);

// Compiles a shader.
GLuint CompileShader(GLuint type, const std::string &source);

// Creates a shader program.
GLuint CreateShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);