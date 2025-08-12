#include "Base.h"


//---------------------------------------------------------------------------------------------------------------
Base::Base(float radius, float speed, Vector2f directionVector)
    : radius_(radius), speed_(speed), directionVector_(directionVector)
{
    bodu_.setRadius(radius);
    bodu_.setOutlineColor(Color::Green); // ���� ����� ������� ������
    bodu_.setOutlineThickness(3);       // ������� ����� ������� ������
    bodu_.setFillColor(Color::Red);    // ���� ������� ������
}
//---------------------------------------------------------------------------------------------------------------
float Base::getRadius()
{
    //return radius_;
    return bodu_.getRadius(); // ������ ������� ����������
}
//---------------------------------------------------------------------------------------------------------------
void Base::draw(RenderWindow& window)
{
    window.draw(bodu_);
}
//---------------------------------------------------------------------------------------------------------------
void Base::eat(float size)
{
    float newRadius = bodu_.getRadius() + size; // ����������� ������ ������
    radius_ = newRadius; // ��������� ������ � Base
    bodu_.setRadius(newRadius); //����������� ������ ������
    bodu_.setOrigin({ newRadius, newRadius });

    speed_ = speed_ - size;
}
//---------------------------------------------------------------------------------------------------------------
Vector2f Base::getPosition()
{
    return bodu_.getPosition();
}
//---------------------------------------------------------------------------------------------------------------