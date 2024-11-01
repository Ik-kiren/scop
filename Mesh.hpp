#pragma once
#include "Scop.hpp"

class Camera;

class Mesh {
 private:
    std::vector<GLfloat>    vertices;
    std::vector<GLuint>     indices;
    std::vector<Vector3>    vecVertices;
    Matrix4                 model;
    Matrix4                 projection;
    Shader                  meshShader;
    unsigned int            texture;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    bool activeTexture;
    double timer;

    bool textureTransition = false;
    double timerTextureTransition = 1.0;

 public:
    Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, Shader meshShader);
    Mesh(Shader meshShader, Object obj);
    ~Mesh();

    void                    InitTexture();
    std::vector<GLfloat>    getVertices();
    std::vector<GLuint>     getIndices();
    float                   getOffsetZ();
    float                   getOffsetY();
    float                   getOffsetX();
    Vector3                 getOffset();
    GLuint                  getVao();
    Matrix4                 *getModel();
    void                    SetModel(Matrix4 newModel);
    void                    bindVao();
    void                    drawMesh(GLFWwindow *window, Camera camera);
};
