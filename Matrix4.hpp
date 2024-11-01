#pragma once
#include "Vector3.hpp"
#include "Vector4.hpp"
#include <string>
#include <iostream>


class Matrix4
{
 public:
    Vector4 matrix[4];
    float matrix_array[16];

    Matrix4();
    Matrix4(float nbr);
    Matrix4(Vector3 v1, Vector3 v2, Vector3 v3, Vector3 v4);
    Matrix4(Vector4 v1, Vector4 v2, Vector4 v3, Vector4 v4);
    ~Matrix4();

    void MatrixArrayInit();
    Vector4& operator[](int index);
    Matrix4 operator*(Matrix4 rhs);
    Vector4 operator*(Vector4 rhs);
    Matrix4 &operator=(Matrix4 const &rhs);

    float *get_array();
};

std::ostream &operator<<(std::ostream &out, Matrix4 &rhs);