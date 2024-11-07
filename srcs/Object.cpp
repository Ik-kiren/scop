#include <cmath>
#include <vector>
#include <iostream>
#include "../includes/Object.hpp"
#include "../includes/Scop.hpp"

Object::Object(std::vector<GLfloat> vertices, std::vector<GLuint> indices, Shader meshShader)
    : vertices(vertices), indices(indices), meshShader(meshShader) {
    timer = 0;
    position = Vector3(0, 0, 0);
    model = Matrix4(1.0f);
    projection = Perspective(60.0f, 1920 / 1200, 0.1f, 100.0f);
    Vector3 newVec;
    for (size_t i = 0; i < this->vertices.size(); i++) {
        newVec[i % 3] = this->vertices[i];
        if (i % 3 == 2) {
            this->vecVertices.push_back(newVec);
        }
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    this->InitTexture();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices.data()) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(*indices.data()) * indices.size(), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            3 * sizeof(float),
            reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    activeTexture = false;
}

Object::Object(Shader meshShader, Mesh mesh)
    : vertices(mesh.GetMeshVertexArray()), meshShader(meshShader) {
    model = Matrix4(1.0f);
    projection = Perspective(60.0f, 1920 / 1200, 0.1f, 100.0f);
    timer = 0;
    position = Vector3(0, 0, 0);
    Vector3 newVec;
    for (size_t i = 0; i < mesh.GetVertices().size(); i++) {
        newVec[i % 3] = mesh.GetVertices()[i];
        if (i % 3 == 2) {
            this->vecVertices.push_back(newVec);
        }
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    this->InitTexture();

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*vertices.data()) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    int strideSize = 6;
    if (mesh.GetComponents() == 2)
        strideSize = 9;
    else if (mesh.GetComponents() == 3)
        strideSize = 11;
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        strideSize * sizeof(float),
        reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    if (mesh.GetComponents() >= 2) {
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            strideSize * sizeof(float),
            reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    if (mesh.GetComponents() == 3) {
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            strideSize * sizeof(float),
            reinterpret_cast<void*>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(
            3,
            3,
            GL_FLOAT,
            GL_FALSE,
            strideSize * sizeof(float),
            reinterpret_cast<void*>(8 * sizeof(float)));
        glEnableVertexAttribArray(3);
    } else {
        glVertexAttribPointer(
            3,
            3,
            GL_FLOAT,
            GL_FALSE,
            strideSize * sizeof(float),
            reinterpret_cast<void*>((strideSize - 3) * sizeof(float)));
        glEnableVertexAttribArray(3);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    activeTexture = false;
}

Object::Object(const Object &obj) {
    position = obj.position;
    vertices = obj.vertices;
    indices = obj.indices;
    vecVertices = obj.vecVertices;
    model = obj.model;
    projection = obj.projection;
    meshShader = obj.meshShader;
    texture = obj.texture;
    VAO = obj.VAO;
    VBO = obj.VBO;
    EBO = obj.EBO;
    activeTexture = obj.activeTexture;
    timer = obj.timer;
    textureTransition = obj.textureTransition;
    timerTextureTransition = obj.timerTextureTransition;
}

Object::~Object() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Object::InitTexture() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("./textures/Triforce.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

std::vector<GLfloat> Object::getVertices() {
    return this->vertices;
}

std::vector<GLuint> Object::getIndices() {
    return this->indices;
}

GLuint  Object::getVao() {
    return this->VAO;
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

void    Object::SetModel(Matrix4 newModel) {
    this->model = newModel;
}

void    Object::bindVao() {
    glBindVertexArray(this->VAO);
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
    meshShader.setMatrix4("model", model);
    meshShader.setMatrix4("view", camera.GetViewMatrix());
    meshShader.setMatrix4("projection", projection);
    meshShader.setVector3("offset", getOffset());
    meshShader.setVector3("cameraPos", camera.GetPosition());
    meshShader.setFloat("timeValue", sin(glfwGetTime()) / 0.3f);
    meshShader.setBool("activeTexture", activeTexture);
    meshShader.setFloat("timerTextureTransition", timerTextureTransition);


    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void    Object::drawMesh(GLFWwindow *window, Camera camera, Vector3 lightPos) {
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
    meshShader.setMatrix4("model", model);
    meshShader.setMatrix4("view", camera.GetViewMatrix());
    meshShader.setMatrix4("projection", projection);
    meshShader.setVector3("offset", getOffset());
    meshShader.setVector3("cameraPos", camera.GetPosition());
    meshShader.setFloat("timeValue", sin(glfwGetTime()) / 0.3f);
    meshShader.setBool("activeTexture", activeTexture);
    meshShader.setFloat("timerTextureTransition", timerTextureTransition);
    meshShader.setVector3("lightPos", lightPos);


    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, vertices.size());
}

void        Object::translate(Vector3 vec) {
    position = position + vec;
   this->model = Translate(this->model, vec);
}

Matrix4     *Object::getModel() {
    return &this->model;
}
