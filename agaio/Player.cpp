#include "Player.h"

#include "Enemy.h"

extern bool gameWin;
extern vector<CircleShape> shsr;
extern vector<Enemy> enemies;

//---------------------------------------------------------------------------------------------------------------
Player::Player()
    : Base(20.f, 400.f, { 0.0f, 0.0f })
{
    bodu_.setPosition({ 400 - 20, 300 - 20 });
}
//---------------------------------------------------------------------------------------------------------------
void Player::setDirection(Vector2f directionVector)
{
    directionVector_ = directionVector;
}
//---------------------------------------------------------------------------------------------------------------
void Player::move(float deltaTime)
{

    if (directionVector_ != Vector2f(0.0f, 0.0f) && gameWin == false)
    {
        float frameSpeed = speed_ * deltaTime;

        directionVector_ = directionVector_.normalized() * frameSpeed;
        for (int i = 0; i < shsr.size(); i++)
        {
            shsr[i].move(directionVector_);
        }

        for (int i = 0; i < enemies.size(); i++)
        {
            enemies[i].moveCamera(directionVector_);
        }
        //directionVector_ = Vector2f(0.0f, 0.0f);
    }
}
//---------------------------------------------------------------------------------------------------------------
void Player::resetRadius()
{
    radius_ = 20.f; // —брос до значени€ по умолчанию
    bodu_.setRadius(radius_); // ќбновление радиуса фигуры
    bodu_.setOrigin({ radius_, radius_ }); // ќбновление центра фигуры
    speed_ = 400.f; // —брос скорости
}
//---------------------------------------------------------------------------------------------------------------