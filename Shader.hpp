#pragma once

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Matrix4.hpp"


class Shader
{
private:
    GLuint programID;

    void checkShaderCompile(GLuint shader, const std::string name);
    void checkProgramCompile(GLuint programID);
    
public:
    Shader(const std::string vertexPath, const std::string fragmentPath);
    ~Shader();

    void use();
    void setFloat(const std::string name, float nbr);
    void setInt(const std::string name, int nbr);
    void setMatrix4(const std::string name, Matrix4 matrix);
    void setVector3(const std::string name, Vector3 vec);
    void setFloatArray(const std::string name, size_t size, GLfloat *array);
    void setBool(const std::string name, bool state);
};
