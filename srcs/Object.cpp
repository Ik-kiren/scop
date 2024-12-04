#include <cmath>
#include <vector>
#include <iostream>
#include "../includes/Object.hpp"
#include "../includes/Scop.hpp"

Object::Object() {}

Object::Object(Shader &meshShader, Mesh *mesh)
    : vertices(mesh->GetMeshVertexArray()), meshShader(meshShader), mesh(mesh) {
    model = Matrix4(1.0f);
    projection = Perspective(30.0f, 1920 / 1200, 0.1f, 100.0f);
    // projection = Orthographique(-19, 19, -19, 19, 0, 40);
    this->color = Vector4(1.0, 1.0, 1.0, 0.5);
    timer = 0;
    position = Vector3(0, 0, 0);
    Vector3 newVec;
    for (size_t i = 0; i < vertices.size(); i++) {
        newVec[i % 3] = vertices[i];
        if (i % 3 == 2) {
            this->vecVertices.push_back(newVec);
        }
    }
    activeTexture = false;
}

Object::Object(Shader &meshShader, Mesh *mesh, Vector3 color)
    : vertices(mesh->GetMeshVertexArray()), meshShader(meshShader), mesh(mesh) {
    model = Matrix4(1.0f);
    projection = Perspective(30.0f, 1920 / 1200, 0.1f, 100.0f);
    // projection = Orthographique(-19, 19, -19, 19, 0, 40);
    this->color = Vector4(color.x, color.y, color.z, 1.0);
    timer = 0;
    position = Vector3(0, 0, 0);
    Vector3 newVec;
    for (size_t i = 0; i < vertices.size(); i++) {
        newVec[i % 3] = vertices[i];
        if (i % 3 == 2) {
            this->vecVertices.push_back(newVec);
        }
    }
    activeTexture = false;
}

Object::Object(Shader &meshShader, Mesh *mesh, Vector4 color)
    : vertices(mesh->GetMeshVertexArray()), meshShader(meshShader), mesh(mesh), color(color) {
    model = Matrix4(1.0f);
    projection = Perspective(30.0f, 1920 / 1200, 0.1f, 100.0f);
    // projection = Orthographique(-19, 19, -19, 19, 0, 40);
    timer = 0;
    position = Vector3(0, 0, 0);
    Vector3 newVec;
    for (size_t i = 0; i < vertices.size(); i++) {
        newVec[i % 3] = vertices[i];
        if (i % 3 == 2) {
            this->vecVertices.push_back(newVec);
        }
    }
    activeTexture = false;
}

Object::Object(const Object &obj) : mesh(obj.mesh) {
    position = obj.position;
    vertices = obj.vertices;
    indices = obj.indices;
    vecVertices = obj.vecVertices;
    model = obj.model;
    projection = obj.projection;
    meshShader = obj.meshShader;
    activeTexture = obj.activeTexture;
    color = obj.color;
    timer = obj.timer;
    textureTransition = obj.textureTransition;
    timerTextureTransition = obj.timerTextureTransition;
}

Object::~Object() {
}

std::vector<GLfloat> Object::getVertices() {
    return this->vertices;
}

std::vector<GLuint> Object::getIndices() {
    return this->indices;
}

GLuint  Object::getVao() {
    return this->mesh->getVAO();
}

float Object::getOffsetZ() {
    float minZ = vecVertices[0].z;
    float maxZ = vecVertices[0].z;
    for (size_t i = 0; i < vecVertices.size(); i++) {
        if (vecVertices[i].z < minZ)
            minZ = vecVertices[i].z;
        if (vecVertices[i].z > maxZ)
            maxZ = vecVertices[i].z;
    }
    return maxZ / 2 + minZ / 2;
}

float Object::getOffsetY() {
    float minY = vecVertices[0].y;
    float maxY = vecVertices[0].y;
    for (size_t i = 0; i < vecVertices.size(); i++) {
        if (vecVertices[i].y < minY)
            minY = vecVertices[i].y;
        if (vecVertices[i].y > maxY)
            maxY = vecVertices[i].y;
    }
    return maxY / 2 + minY / 2;
}

float Object::getOffsetX() {
    float minX = vecVertices[0].x;
    float maxX = vecVertices[0].x;
    for (size_t i = 0; i < vecVertices.size(); i++) {
        if (vecVertices[i].x < minX)
            minX = vecVertices[i].x;
        if (vecVertices[i].x > maxX)
            maxX = vecVertices[i].x;
    }
    return maxX / 2 + minX / 2;
}

Vector3 Object::getOffset() {
    Vector3 offsetVector(getOffsetX(), getOffsetY(), getOffsetZ());
    return offsetVector;
}

Vector3 Object::GetPosition() {
    return position;
}

Vector4 Object::getColor() {
    return color;
}

void    Object::SetModel(Matrix4 newModel) {
    this->model = newModel;
}

void    Object::bindVao() {
    glBindVertexArray(this->mesh->getVAO());
}

void    Object::drawMesh(GLFWwindow *window, Camera camera) {
    if (timer < 3)
        timer += 0.01;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && timer > 1) {
        textureTransition = true;
        timerTextureTransition = 0;
        if (activeTexture)
            activeTexture = false;
        else
            activeTexture = true;
        timer = 0;
    }
    if (textureTransition) {
        if (timerTextureTransition <= 1 && timerTextureTransition >= 0)
            timerTextureTransition += 0.01;
        else
            textureTransition = false;
    }

    meshShader.use();

    meshShader.setVector4("newColor", color);
    meshShader.setMatrix4("model", model);
    meshShader.setMatrix4("view", camera.GetViewMatrix());
    meshShader.setMatrix4("projection", projection);
    meshShader.setVector3("offset", getOffset());
    meshShader.setVector3("cameraPos", camera.GetPosition());
    meshShader.setFloat("timeValue", sin(glfwGetTime()) / 0.3f);
    meshShader.setBool("activeTexture", activeTexture);
    meshShader.setFloat("timerTextureTransition", timerTextureTransition);

    glBindTexture(GL_TEXTURE_2D, mesh->getTexture());
    glBindVertexArray(mesh->getVAO());
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void Object::TextureTransition() {
    activeTexture = (activeTexture) ? false : true;
}

void    Object::drawMesh(GLFWwindow *window, Camera camera, Vector3 lightPos) {
    (void)window;
    /*if (timer < 3)
        timer += 0.01;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS && timer > 1) {
        textureTransition = true;
        timerTextureTransition = 0;
        if (activeTexture)
            activeTexture = false;
        else
            activeTexture = true;
        timer = 0;
    }
    if (textureTransition) {
        if (timerTextureTransition <= 1 && timerTextureTransition >= 0)
            timerTextureTransition += 0.01;
        else
            textureTransition = false;
    }*/
    meshShader.use();
    meshShader.setVector4("newColor", color);
    meshShader.setMatrix4("model", model);
    meshShader.setMatrix4("view", camera.GetViewMatrix(Vector3(0, 0, 0)));
    meshShader.setMatrix4("projection", projection);
    meshShader.setVector3("offset", getOffset());
    meshShader.setVector3("cameraPos", camera.GetPosition());
    meshShader.setFloat("timeValue", sin(glfwGetTime()) / 0.3f);
    meshShader.setBool("activeTexture", activeTexture);
    meshShader.setFloat("timerTextureTransition", timerTextureTransition);
    meshShader.setVector3("lightPos", lightPos);

    glBindTexture(GL_TEXTURE_2D, mesh->getTexture());
    glBindVertexArray(mesh->getVAO());
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void        Object::translate(Vector3 vec) {
    position = position + vec;
    this->model = Translate(this->model, vec);
}

void        Object::rotate(Vector3 vec) {
    this->model = Rotate(this->model, M_PI / 512, vec);
}

void        Object::rotate(Vector3 vec, float speed) {
    this->model = Rotate(this->model, M_PI / speed * 2, vec);
}

Matrix4     *Object::getModel() {
    return &this->model;
}

Object  *Object::clone() {
    return new Object(this->meshShader, this->mesh);
}

Object &Object::operator=(const Object &rhs) {
    this->position = rhs.position;
    this->vertices = rhs.vertices;
    this->indices = rhs.indices;
    this->vecVertices = rhs.vecVertices;
    this->model = rhs.model;
    this->projection = rhs.projection;
    this->meshShader = rhs.meshShader;
    this->mesh = rhs.mesh;
    this->activeTexture = rhs.activeTexture;
    this->timer = rhs.timer;
    this->textureTransition = rhs.textureTransition;
    this->timerTextureTransition = rhs.timerTextureTransition;
    this->color = rhs.color;
    return *this;
}


void    Object::print() {
    std::cout << "pos = " << position << std::endl;
    std::cout << "VAO = " << mesh->getVAO() << std::endl;
    std::cout << "vertices = " << vertices.size() << std::endl;
    std::cout << "indices = "<< indices.size() << std::endl;
    std::cout << "vecvertices = " << vecVertices.size() << std::endl;
    std::cout << "model = " << model << std::endl;
    std::cout << "projection = " << projection << std::endl;
    std::cout << "meshShader = " << meshShader.programID << std::endl;
    std::cout << "activatedTexture = " << activeTexture  << std::endl;
    std::cout << "timer = " << timer  << std::endl;
    std::cout << "textureTransition = " << textureTransition  << std::endl;
    std::cout << "timerTexture = " << timerTextureTransition  << std::endl;
    std::cout << "color = " << color << std::endl;
    std::cout << std::endl;
}
