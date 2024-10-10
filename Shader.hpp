#ifndef SHADER_HPP
#define SHADER_HPP

#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
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
    void set_float(const std::string name, float nbr);
    void set_matrix4(const std::string name, Matrix4 matrix);
};

#endif