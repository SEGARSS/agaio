#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <vector>

using namespace std;
using namespace sf;

//------------------------------------------------------------------
class Base
{
public:
    Base(float radius, float speed, Vector2f directionVector);

    float getRadius();

    void draw(RenderWindow& window);

    void eat(float size = 2.f);

    Vector2f getPosition();

    virtual void setDirection(Vector2f directionVector) = 0;
    virtual Vector2f getDirection();
    virtual void move(float deltaTime) = 0;

    CircleShape bodu_;
    float radius_;
    float speed_;
    Vector2f directionVector_;

private:
};
//------------------------------------------------------------------
