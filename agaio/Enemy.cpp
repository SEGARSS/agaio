#include "Enemy.h"
#include <random>

extern bool gameWin;
extern vector<CircleShape> shsr;
extern vector<Enemy> enemies;

//---------------------------------------------------------------------------------------------------------------
Enemy::Enemy()
    : Base(20.f, 200.f, { 0.0f, 0.0f })
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 400);

    bodu_.setPosition({ static_cast<float>(distrib(gen)), static_cast<float>(distrib(gen)) });
}
//---------------------------------------------------------------------------------------------------------------
void Enemy::setDirection(Vector2f directionVector)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(-400, 400);

    //Пройтись ещё раз 
    if (directionVector.length() <= 200)
    {
        directionVector_ = directionVector;
    }
    else
    {
        //Пример нахождения минимального элемента в векторе.
        /*Пишем ноль чтобы исключить проверку на ноль, и начать с перво элемента в векторе.*/
        Vector2f closestSphere = shsr[0].getPosition() - bodu_.getPosition();

        for (int i = 1; i < shsr.size(); i++)
        {
            Vector2f posmin = shsr[i].getPosition() - bodu_.getPosition();

            if (posmin.length() < closestSphere.length())
            {
                closestSphere = posmin;
            }
        }
        directionVector_ = closestSphere;
    }
}
//---------------------------------------------------------------------------------------------------------------
void Enemy::move(float deltaTime)
{
    float frameSpeed = speed_ * deltaTime;
    Vector2f pos = directionVector_.normalized() * frameSpeed;
    bodu_.move(pos);
    directionVector_ = Vector2f(0.0f, 0.0f);
}
//---------------------------------------------------------------------------------------------------------------
void Enemy::moveCamera(Vector2f& directionVector)
{
    bodu_.move(directionVector);
}
//---------------------------------------------------------------------------------------------------------------