// Shader.cpp
// Handles loading in shaders.
#include "Shader.hpp"

// Loads in a shader .glsl file.
std::string LoadShaderAsString(const std::string &fileName) {
    // Resulting shader program loaded as a single string
    std::string result = "";

    std::string line = "";

    std::ifstream myFile(fileName.c_str());

    if(myFile.is_open()) {
        while(std::getline(myFile, line)) {
            result+= line + '\n';
        }
        myFile.close();
    }
    return result;
}

// Compiles the shader.
GLuint CompileShader(GLuint type, const std::string &source) {
    // Compile our shaders.
    GLuint shaderObject;

    // Based on the type passed in, we create a shader object for that type.
    if (GL_VERTEX_SHADER == type){
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    } else if (GL_FRAGMENT_SHADER == type) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    }

    const char* src = source.c_str();
    // The source of our shader
    glShaderSource(shaderObject, 1, &src, nullptr);
    // Now we compile.
    glCompileShader(shaderObject);

    // Check the result of compiling.
    int result;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    if (GL_FALSE == result) {
        int length;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
        char* errorMessages = new char[length];
        glGetShaderInfoLog(shaderObject, length, &length, errorMessages);

        if (GL_VERTEX_SHADER == type) {
            std::cout << "Error: GL_VERTEX_SHADER compilation failed: \n" << errorMessages << "\n";
        } else if (GL_FRAGMENT_SHADER == type) {
            std::cout << "Error: GL_FRAGMENT_SHADER compilation failed: \n" << errorMessages << "\n";
        }

        // Reclaim memory.
        delete[] errorMessages;

        // Delete our broken shader.
        glDeleteShader(shaderObject);

        return 0;
    }
    return shaderObject;
}

// Creates a shader program.
GLuint CreateShaderProgram(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
    // Create a new program object.
    GLuint programObject = glCreateProgram();

    // Compile our shaders.
    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    // Link our two shaders together.
    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    // Validate our program.
    glValidateProgram(programObject);

    // programObject created. Detach and delete shaders.
    glDetachShader(programObject, myVertexShader);
    glDetachShader(programObject, myFragmentShader);

    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    return programObject;
}