#ifndef SCOP_HPP
#define SCOP_HPP

#include <iostream>
#include <fstream>
#include <ostream>
#include <sstream>
#include <math.h>
#include <stdio.h>
#include <vector>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix4.hpp"
#include "Camera.hpp"
#include "Mesh.hpp"

void Parser(std::string fileName, std::vector<GLfloat> *vertices, std::vector<GLuint> *indices);

// ScopMaths.cpp
Vector3 cross(Vector3 vec1, Vector3 vec2);
float magnitude(Vector3 vec);
Vector3 normalized(Vector3 vec);
Matrix4 Translate(Matrix4 mat, Vector3 vec);
Matrix4 Transposed(Matrix4 mat);
Matrix4 lookAt(Vector3 position, Vector3 target, Vector3 upVector);
Matrix4 Rotate(Matrix4 mat, float radians, Vector3 axis);
Matrix4 Perspective(float fov, float aspectRatio, float near, float far);
Matrix4 Orthographique(float left, float right, float bot, float top, float near, float far);

#endif