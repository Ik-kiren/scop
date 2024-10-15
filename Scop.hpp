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

void Parser(std::string fileName, std::vector<GLfloat> *vertices, std::vector<GLuint> *indices);

#endif