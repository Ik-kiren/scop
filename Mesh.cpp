#include "Mesh.hpp"

Mesh::Mesh(std::vector<GLfloat> vertices, std::vector<GLuint> indices, Shader meshShader, Matrix4 *view, Matrix4 *projection)
    : vertices(vertices), indices(indices), meshShader(meshShader), view(view), projection(projection)
{
    model = Matrix4(1.0f);
    Vector3 newVec;
    for (size_t i = 0; i < this->vertices.size(); i++)
    {
        newVec[i % 3] = this->vertices[i];
        if (i % 3 == 2)
        {
            this->vecVertices.push_back(newVec);
        }
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
    
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
            (void*)0
    );
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Mesh::~Mesh()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

std::vector<GLfloat>	Mesh::getVertices()
{
    return this->vertices;
}

std::vector<GLuint>		Mesh::getIndices()
{
    return this->indices;
}

GLuint  Mesh::getVao()
{
    return this->VAO;
}

float Mesh::getOffsetZ()
{
    float minZ = vecVertices[0].z;
    float maxZ = vecVertices[0].z;
    for (size_t i = 0; i < vecVertices.size(); i++)
    {
        if (vecVertices[i].z < minZ)
            minZ = vecVertices[i].z;
        if (vecVertices[i].z > maxZ)
            maxZ = vecVertices[i].z;
        
    }
    return maxZ / 2 + minZ / 2;
}

float Mesh::getOffsetY()
{
    float minY = vecVertices[0].y;
    float maxY = vecVertices[0].y;
    for (size_t i = 0; i < vecVertices.size(); i++)
    {
        if (vecVertices[i].y < minY)
            minY = vecVertices[i].y;
        if (vecVertices[i].y > maxY)
            maxY = vecVertices[i].y;
        
    }
    return maxY / 2 + minY / 2;
}

float Mesh::getOffsetX()
{
    float minX = vecVertices[0].x;
    float maxX = vecVertices[0].x;
    for (size_t i = 0; i < vecVertices.size(); i++)
    {
        if (vecVertices[i].x < minX)
            minX = vecVertices[i].x;
        if (vecVertices[i].x > maxX)
            maxX = vecVertices[i].x;
        
    }
    return maxX / 2 + minX / 2;
}

Vector3 Mesh::getOffset()
{
    Vector3 offsetVector(getOffsetX(), getOffsetY(), getOffsetZ());
    return offsetVector;
}

void    Mesh::bindVao()
{
    glBindVertexArray(this->VAO);
}

void    Mesh::drawMesh()
{
    meshShader.use();

    meshShader.setMatrix4("model", model);
    meshShader.setMatrix4("view", *view);
    meshShader.setMatrix4("projection", *projection);
    meshShader.setVector3("offset", getOffset());
    meshShader.setFloat("timeValue", sin(glfwGetTime()) / 0.3f);

    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, sizeof(*indices.data()) * indices.size(), GL_UNSIGNED_INT, 0);
}

Matrix4     *Mesh::getModel()
{
    return &this->model;
}