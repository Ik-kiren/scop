#include "Shader.hpp"

Shader::Shader(const std::string vertexPath, const std::string shaderPath)
{
    std::string vertexCode; 
    std::string fragmentCode;
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    try
    {
        vertexFile.open(vertexPath);
        fragmentFile.open(shaderPath);

        std::stringstream vertexStream;
        std::stringstream fragmentStream;

        vertexStream << vertexFile.rdbuf();
        fragmentStream << fragmentFile.rdbuf();

        vertexFile.close();
        fragmentFile.close();

        vertexCode = vertexStream.str();
        fragmentCode = fragmentStream.str();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    GLuint vertex;
    GLuint fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkShaderCompile(vertex, "VERTEX");

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkShaderCompile(fragment, "FRAGMENT");

    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    checkProgramCompile(programID);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader(){}

void Shader::use()
{
    glUseProgram(programID);
}

void Shader::set_float(const std::string name, float nbr)
{
    glUniform1f(glGetUniformLocation(programID, name.c_str()), nbr);
}

void Shader::set_matrix4(const std::string name, Matrix4 matrix)
{
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, matrix.get_array());
}

void Shader::checkShaderCompile(GLuint shader, const std::string name)
{
    int success;
    char logs[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, logs);
        std::cout << "SHADER COMPILER ERROR: " << name << "\n" << logs << std::endl;
    }
}

void Shader::checkProgramCompile(GLuint programID)
{
    int success;
    char logs[1024];
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(programID, 1024, NULL, logs);
        std::cout << "PROGRAM LINKING ERROR:" << "\n" << logs << std::endl;
    }
}
