#pragma once
#include <SFML/System.hpp>

struct Math
{
    static float dot(sf::Vector2f v1, sf::Vector2f v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }
    static float dot(sf::Vector3f v1, sf::Vector3f v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    static float hyp(sf::Vector3f vec)
    {
        return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }

};