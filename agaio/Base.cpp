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