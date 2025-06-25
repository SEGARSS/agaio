#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;
using namespace sf;

/*
Размер мира будет 1000х1000
*/

int puls = 20;

/*
Надо будет избавится от бага чёрных дыр =) или сделать фон белыйм, но и в таком случаи будут тогда
белын шалрики =)
*/

//--------------------------------------------------------------------------------
CircleShape getShsr(float x, float y)//функция генерирует кружок, и устанавливает все необходимые параметры
{
    CircleShape shsr;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 255);

    shsr.setRadius(5);                  // Радиус
    shsr.setOutlineColor(Color::Cyan); // Цвет линии обводки фигуры
    shsr.setOutlineThickness(1);      // Толщина линии обводки фигуры
    shsr.setFillColor( { static_cast<uint8_t>(distrib(gen)), static_cast<uint8_t>(distrib(gen)), static_cast<uint8_t>(distrib(gen)) }); // Цвет заливки фигуры
    shsr.setPosition({ x, y}); 

    return shsr;
}
//---------------------------------------------------------------------------------------
enum class Direction//перечисление, хранит варианты направления движения
{
    stop,

    up,
    down,
    left,
    right
};
//----------------------------------------------------------------------------------------
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML window");
     
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, 1000);
    uniform_int_distribution<> distrib2(1, 100);

    vector<CircleShape> shsr;

    for (int i = 0; i < puls; i++)
    {
        shsr.push_back(getShsr(distrib(gen), distrib(gen)));
    }

    CircleShape glShsr;
    glShsr.setRadius(20);                  // Радиус
    glShsr.setOutlineColor(Color::Green); // Цвет линии обводки фигуры
    glShsr.setOutlineThickness(3);       // Толщина линии обводки фигуры
    glShsr.setFillColor(Color::Red);    // Цвет заливки фигуры
    glShsr.setPosition({ 400 - 20, 300 - 20 });

    Clock frameClock; //счетчик, чтобы считать время, за которое выводится кадр(frame), 1 сек / на это время = fps (кадры в сек)

    float speed = 400.f; // скорость движения пикселей в секунду
    Vector2f directionVector(0.0f, 0.0f); // вектор направления, сделали взамен enum direction

    window.setFramerateLimit(60); //ограничим fps чтобы не было рывков в движении шариков 
    window.setVerticalSyncEnabled(true); //оптимизация тоже для увеличения плавности
    // над плавностью еще поработаем дальше

    // Start the game loop
    while (window.isOpen())
    {
        // переменная в которой считаем время за которое выводится кадр
        // понятие deltaTime есть во многих движках, и везде оно обозначает одно и то же - время за которое выводится кадр
        float deltaTime = frameClock.getElapsedTime().asSeconds();

        //std::cout << deltaTime << std::endl;
        cout << "fps: " << 1.f / deltaTime << "\n"; // просто для наглядности сколько сейчас у нас fps
        frameClock.restart(); // важно сбрасывать счетчик каждый кадр, чтобы считать именно время кадра

        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                window.close();

            if (event->is<Event::KeyPressed>())
            {
                if (Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                {
                    directionVector.y = -1.0f;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                {
                    directionVector.x = -1.0f;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                {
                    directionVector.x = 1.0f;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                {
                    directionVector.y = 1.0f;
                }
            }
        }

        // тут у нас появляется понятие нормализации вектора
        // нормализация это приведение вектора к длине равной 1, но сохраняя направление вектора
        // делаем это чтобы наши шарики двигались с одинковой скоростью по диагонали
        // длина вектора считается как квадратный корень от суммы квадратов x и y
        // sqrt(x^2 + y^2) где sqrt - квадратный корень, ^2 - возведение в степень 2 или в квадрат
        // на пример x = 1, y = 0 => sqrt(1^2 + 0^2) = sqrt(1 + 0) = sqrt(1) = 1
        // на пример x = 1, y = 1 => sqrt(1^2 + 1^2) = sqrt(1 + 1) = sqrt(2) = 1.41 , т.е. почти на 40% быстрее двигаться по диагонали
        // после нормализации получаем x = 0.71 y = 0.71, и длина вектора примерно 1
        if (directionVector != Vector2f(0.0f, 0.0f))
        {
            //считаем на сколько нужно сдвинуть шарики, учитывая время кадра
            //если скорость 400 пикселей в сек, а кадр выводится за 0,0004 секунды, 
            //значит нам нужно переместить в текущем кадре шарики всего на 1,6 пикселя, и тогда за 1 секунду мы переместимся на 400 пикселей
            float frameSpeed = speed * deltaTime;
            // { 1, 0 } -> { 1, 0 }
            // { 1, 1 } -> { 0.71, 0.71 }
            //// { 0.24, 0 }
            // нормализируем вектор направления и только после этого умножаем на скорость в текущем кадре
            directionVector = directionVector.normalized() * frameSpeed;
            for (int i = 0; i < shsr.size(); i++)
            {
                shsr[i].move(directionVector);
            }
            directionVector = Vector2f(0.0f, 0.0f);// не забываем сбросить вектор направления, иначе бует двигаться даже после отпускания кнопки движения
        }

        //цикл определения столкновения с шариками
        for (int i = 0; i < shsr.size(); i++)
        {
            //определяем расстояние между нашим шариком и шариком из вектора
            //такой вектор можем получить просто отняв позицию шарика из вектора от позиции нашего нарика
            Vector2f rasoyanie = glShsr.getPosition() - shsr[i].getPosition();
            //считаем сумму радиусов нашего шарика и шарика из вектора
            int sumRad = glShsr.getRadius() + shsr[i].getRadius();
            //если расстояние между шариками меньше или равно сумме радиусов - значит шарики столкнулись
            if (rasoyanie.length() <= sumRad) 
            {
                cout << "Collision detected!" << endl;
            }

            /*if (glShsr.getPosition() == shsr[i].getPosition())
            {
                shsr.push_back(getShsr(distrib(gen), distrib(gen)));
            }*/
        }

        //Пытаюсь переместить шарик....
        for (int i = 0; i < shsr.size(); i++)
        {
            if (glShsr.getPosition() == shsr[i].getPosition())
            {
                //shsr[i].setPosition({ distrib2(gen), distrib2(gen) });//Сужение просит, надо ли?
                shsr.push_back(getShsr(distrib(gen), distrib(gen)));// Ничо не двигает
            }
        }
        

        // Clear screen
        window.clear();

        for (int i = 0; i < shsr.size(); i++)
        {
            window.draw(shsr[i]);
        }

        window.draw(glShsr);

        // Update the window
        window.display();
    }
}