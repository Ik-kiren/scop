#pragma once
#include "Vector3.hpp"
#include <ostream>

class Vector4
{
 public:
    float x;
    float y;
    float z;
    float w;


    Vector4();
    Vector4(float x, float y, float z, float w);
    Vector4(Vector3 vec);
    Vector4(Vector3 vec, float nbr);
    ~Vector4();

    Vector4 &operator=(Vector4 const &rhs);
    float &operator[](int index);
    Vector4 &operator+(Vector4 rhs);
    Vector4 &operator*(float rhs);
};

std::ostream &operator<<(std::ostream &out, Vector4 &rhs);