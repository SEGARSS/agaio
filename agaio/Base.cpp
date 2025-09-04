#include "Base.h"


//---------------------------------------------------------------------------------------------------------------
Base::Base(float radius, float speed, Vector2f directionVector)
    : radius_(radius), speed_(speed), directionVector_(directionVector)
{
    bodu_.setRadius(radius);
    bodu_.setOutlineColor(Color::Green); // Цвет линии обводки фигуры
    bodu_.setOutlineThickness(3);       // Толщина линии обводки фигуры
    bodu_.setFillColor(Color::Red);    // Цвет заливки фигуры
}
//---------------------------------------------------------------------------------------------------------------
float Base::getRadius()
{
    //return radius_;
    return bodu_.getRadius(); // Вторйо вариант объявления
}
//---------------------------------------------------------------------------------------------------------------
void Base::draw(RenderWindow& window)
{
    window.draw(bodu_);
}
//---------------------------------------------------------------------------------------------------------------
void Base::eat(float size)
{
    float newRadius = bodu_.getRadius() + size; // Увеличиваем радиус шарика
    radius_ = newRadius; // Обновляем радиус в Base
    bodu_.setRadius(newRadius); //Увеличиваем радиус шарика
    bodu_.setOrigin({ newRadius, newRadius });

    speed_ = speed_ - size;
}
//---------------------------------------------------------------------------------------------------------------
Vector2f Base::getPosition()
{
    return bodu_.getPosition();
}
//---------------------------------------------------------------------------------------------------------------
Vector2f Base::getDirection()
{
    return directionVector_;
}
//---------------------------------------------------------------------------------------------------------------
float Base::getSpeed()
{
    return speed_;
}
//---------------------------------------------------------------------------------------------------------------
void Base::setSpeed(float speed)
{
     defaultSpeed = speed_;
     speed_ = speed;
}
//---------------------------------------------------------------------------------------------------------------
void Base::increaseSpeedUpTime(int time) //Прибавляем (+ к таймеру)
{
    speedUpTime += std::chrono::milliseconds(time);
}
//---------------------------------------------------------------------------------------------------------------
void Base::decreaseSpeedUpTime(int time)//Уменьшаем (- к таймеру)
{
    if (speedUpTime > std::chrono::milliseconds(time))
    {
        speedUpTime -= std::chrono::milliseconds(time);
    }
    else if (speedUpTime > std::chrono::milliseconds(0))
    {
        speedUpTime = std::chrono::milliseconds(0);
        speed_ = defaultSpeed;
    }
}
//---------------------------------------------------------------------------------------------------------------
int Base::getSpeedUpTime()
{
    return speedUpTime.count();
}
//---------------------------------------------------------------------------------------------------------------
