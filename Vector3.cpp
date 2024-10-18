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

Vector3 &Vector3::operator*(float rhs)
{
    x = x * rhs;
    y = y * rhs;
    z = z * rhs;
    return *this;
}

float Vector3::operator*(Vector3 rhs)
{
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

Vector3 &Vector3::operator/(float rhs)
{
    if (rhs == 0)
        return *this;
    x = x / rhs;
    y = y / rhs;
    z = z / rhs;
    return *this;
}

Vector3 &Vector3::operator-(Vector3 rhs)
{
    x = x - rhs.x;
    y = y - rhs.y;
    z = z - rhs.z;
    return *this;
}

Vector3 &Vector3::operator+(Vector3 rhs)
{
    x = x + rhs.x;
    y = y + rhs.y;
    z = z + rhs.z;
    return *this;
}

std::ostream &operator<<(std::ostream &out, Vector3 &rhs)
{
    out << "<";
    for (int i = 0; i < 3; i++)
    {
        out << rhs[i] << ", ";
    }
    out << ">" << std::endl;
    return out;
}
