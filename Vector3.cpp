#include "Vector3.hpp"

Vector3::Vector3()
{
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
};

Vector3::Vector3(Vector3 *vec)
{
    this->x = vec->x;
    this->y = vec->y;
    this->z = vec->z;
};

Vector3::~Vector3()
{
};

float &Vector3::operator[](int index)
{
    if (index < 0 || index > 2)
    {
        std::string str = "Vector3: " + std::to_string(index) + " out of range";
        throw std::out_of_range(str);
    }
    if (index == 0)
        return x;
    if (index == 1)
        return y;
    return z;
}