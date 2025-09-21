#pragma once

#include <cmath>

class Vector2
{
public:
    float x=0, y=0;
    Vector2(float x=0, float y=0) : x(x), y(y) {}
    ~Vector2()=default;

    Vector2 operator+(const Vector2& other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator/(float scalar) const
    {
        if (scalar != 0)
            return Vector2(x / scalar, y / scalar);
        return Vector2(0, 0);
    }

    void operator+=(const Vector2& other)
    {
        x += other.x;
        y += other.y;
    }

    Vector2 operator-(const Vector2& other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    void operator-=(const Vector2& other)
    {
        x -= other.x;
        y -= other.y;
    }

    Vector2 operator*(float scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    void operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
    }

    bool operator==(const Vector2& other) const
    {
        return x == other.x && y == other.y;
    }

    float length() const
    {
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalized() const
    {
        float len = length();
        if (len > 0)
            return Vector2(x / len, y / len);
        return Vector2(0, 0);
    }
};
