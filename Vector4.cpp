#include "Vector4.hpp"

Vector4::Vector4()
{

}

Vector4::Vector4(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4::Vector4(Vector3 vec)
{
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = 1.0;
}

Vector4::~Vector4()
{

}

Vector4 &Vector4::operator=(Vector4 const &rhs)
{
    this->x = rhs.x;
    this->y = rhs.y;
    this->z = rhs.z;
    this->w = rhs.w;
    return *this;
}

float &Vector4::operator[](int index)
{
    if (index < 0 || index > 3)
    {
        std::string str = "Vector4: " + std::to_string(index) + " out of range";
        throw std::out_of_range(str);
    }
    if (index == 0)
        return x;
    if (index == 1)
        return y;
    if (index == 2)
        return z;
    return w;
}