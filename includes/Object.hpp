#pragma once
#include <vector>
#include "./Vector3.hpp"
#include "./Matrix4.hpp"
#include "./Shader.hpp"
#include "./Mesh.hpp"
#include "./Scop.hpp"
#include "./Camera.hpp"

class Object {
 private:
    std::vector<GLfloat>      vertices;
    std::vector<GLuint>       indices;
    std::vector<Vector3>      vecVertices;
    Matrix4                   model;
    Matrix4                   projection;
    Shader                    meshShader;
    unsigned int              texture;

    GLuint                    VAO;
    GLuint                    VBO;
    GLuint                    EBO;

    Vector3                   position;

    bool                      activeTexture;
    double                    timer;

    bool                      textureTransition = false;
    double                    timerTextureTransition = 1.0;

 public:
    Object(std::vector<GLfloat> vertices, std::vector<GLuint> indices, Shader meshShader);
    Object(Shader meshShader, Mesh mesh);
    Object(const Object &obj);
    ~Object();

    void                    InitTexture();
    std::vector<GLfloat>    getVertices();
    std::vector<GLuint>     getIndices();
    float                   getOffsetZ();
    float                   getOffsetY();
    float                   getOffsetX();
    Vector3                 getOffset();
    Vector3                 GetPosition();
    GLuint                  getVao();
    Matrix4                 *getModel();
    void                    SetModel(Matrix4 newModel);
    void                    bindVao();
    void                    translate(Vector3 vec);
    void                    drawMesh(GLFWwindow *window, Camera camera);
    void                    drawMesh(GLFWwindow *window, Camera camera, Vector3 lightPos);
};
