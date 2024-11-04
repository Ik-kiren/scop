#include <cmath>
#include <vector>
#include <iostream>
#include "../includes/Mesh.hpp"
#include "../includes/Scop.hpp"

Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, Shader meshShader)
    : vertices(vertices), indices(indices), meshShader(meshShader) {
    timer = 0;
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

Mesh::Mesh(Shader meshShader, Object obj)
    : vertices(obj.GetMeshVertexArray()), meshShader(meshShader) {
    model = Matrix4(1.0f);
    projection = Perspective(60.0f, 1920 / 1200, 0.1f, 100.0f);
    timer = 0;
    Vector3 newVec;
    for (size_t i = 0; i < obj.GetVertices().size(); i++) {
        newVec[i % 3] = obj.GetVertices()[i];
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
    int strideSize = 3;
    if (obj.GetComponents() == 2)
        strideSize = 6;
    else if (obj.GetComponents() == 3)
        strideSize = 8;
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        strideSize * sizeof(float),
        reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    if (obj.GetComponents() >= 2) {
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            strideSize * sizeof(float),
            reinterpret_cast<void*>(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    if (obj.GetComponents() == 3) {
        glVertexAttribPointer(
            2,
            2,
            GL_FLOAT,
            GL_FALSE,
            strideSize * sizeof(float),
            reinterpret_cast<void*>(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    activeTexture = false;
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::InitTexture() {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("Triforce.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
}

std::vector<GLfloat> Mesh::getVertices() {
    return this->vertices;
}

std::vector<GLuint> Mesh::getIndices() {
    return this->indices;
}

GLuint  Mesh::getVao() {
    return this->VAO;
}

float Mesh::getOffsetZ() {
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

float Mesh::getOffsetY() {
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

float Mesh::getOffsetX() {
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

Vector3 Mesh::getOffset() {
    Vector3 offsetVector(getOffsetX(), getOffsetY(), getOffsetZ());
    return offsetVector;
}

void    Mesh::SetModel(Matrix4 newModel) {
    this->model = newModel;
}

void    Mesh::bindVao() {
    glBindVertexArray(this->VAO);
}

void    Mesh::drawMesh(GLFWwindow *window, Camera camera) {
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

Matrix4     *Mesh::getModel() {
    return &this->model;
}