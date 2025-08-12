#pragma once

#include "Base.h"


//---------------------------------------------------------------------------------------------------------------
class Enemy : public Base
{
public:
    Enemy();

    void setDirection(Vector2f directionVector) override;

    void move(float deltaTime) override;

    void moveCamera(Vector2f& directionVector);

private:
};
//---------------------------------------------------------------------------------------------------------------