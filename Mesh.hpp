#pragma once
#include "Scop.hpp"

class Mesh
{
private:
    std::vector<GLfloat>	vertices;
    std::vector<GLuint>		indices;
	std::vector<Vector3>	vecVertices;
    Matrix4                 model;
    Matrix4                 *projection;
    Matrix4                 *view;
    Shader					meshShader;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
public:
    Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, Shader meshShader, Matrix4 *view, Matrix4 *projection);
    ~Mesh();

	std::vector<GLfloat>	getVertices();
	std::vector<GLuint>		getIndices();
	float	getOffsetZ();
    float   getOffsetY();
    float   getOffsetX();
    Vector3 getOffset();
    GLuint  getVao();
    Matrix4 *getModel();
    void    SetModel(Matrix4 newModel);
    void    bindVao();
    void    drawMesh();
};
