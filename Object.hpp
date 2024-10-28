#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <GL/glew.h>
#include <stdio.h>
#include <vector>

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

    // 0: vertex, 1 : normale, 2: texture
    int components;
 public:
    Object(std::string str);
    ~Object();

    void MeshCheckLineVertice(char *line);
    void MeshCheckLineFace(char *line);

    std::vector<GLfloat>    GetMeshVertexArray();
    std::vector<GLfloat>    GetVertices();
    int                     GetComponents();

    void MeshGetVertice(char *line);
    void MeshGetNormalVertice(char *line);
    void MeshGetTextureVertice(char *line);
    void MeshGetFace(char *line);
    void MeshParser(std::string fileName);
};