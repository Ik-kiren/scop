#pragma once
#include <stdexcept>

class Vector3
{
 public:
    float x;
    float y;
    float z;


    Vector3();
    Vector3(float x, float y, float z);
    Vector3(Vector3 *vec);
    ~Vector3();

    float &operator[](int index);
};
