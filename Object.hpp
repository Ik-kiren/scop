#pragma once
#include "Scop.hpp"

class Object
{
private:
    std::vector<GLfloat> vertices;

    std::vector<GLfloat> normalVertices;
    std::vector<GLfloat> textureVertices;

    std::vector<GLuint> faces;

    std::vector<GLuint> verticesIndices;
    std::vector<GLuint> normalIndices;
    std::vector<GLuint> textureIndices;

    std::vector<GLfloat> meshVertexArray;
public:
    Object(std::string str);
    ~Object();

    void CheckLineVertice(char *line);
    void CheckLineFace(char *line);
    void GetVertice(char *line);
    void GetFace(char *line);
    void Parser(std::string fileName);
};