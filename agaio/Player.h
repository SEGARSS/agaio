#pragma once

#include "Base.h"

//---------------------------------------------------------------------------------------------------------------
class Player : public Base
{
public:
    Player();

    void setDirection(Vector2f directionVector) override;
    void move(float deltaTime) override;

    void resetRadius();

private:
};
//---------------------------------------------------------------------------------------------------------------