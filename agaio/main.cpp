#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <chrono>
#include <vector>

using namespace std;
using namespace sf;

//--------------------------------------------------------------------------------
// Функция для генерации случайных чисел

/*
Размер мира будет 1000х1000
*/

/*
 - рефакторинг игрока (создать класс, перенести туда все переменные и методы)
 - создать класс для ботов
    - случайное направление движения, если игрока нет рядом
    - следовать за игроком, если он рядом
 - доработать уменьшение мира при увеличении размера игрока
*/

int puls = 50;
int enemyCount = 5;
int ochki = 0;

class Enemy;

vector<CircleShape> shsr;
vector<Enemy> enemies;


bool gameWin = false;


//----------------------------------------------------------------------------------------------------------------------------------------------------------------
CircleShape getShsr(float x, float y)//функция генерирует кружок, и устанавливает все необходимые параметры
{
    CircleShape shsr;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 255);


    shsr.setRadius(5);                  // Радиус
    shsr.setOutlineColor(Color::Cyan); // Цвет линии обводки фигуры
    shsr.setOutlineThickness(1);      // Толщина линии обводки фигуры
    shsr.setFillColor({ static_cast<uint8_t>(distrib(gen)), static_cast<uint8_t>(distrib(gen)), static_cast<uint8_t>(distrib(gen)) }); // Цвет заливки фигуры
    shsr.setPosition({ x, y });

    return shsr;
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class Bass
{
public:
    Bass(float radius, float speed, Vector2f directionVector)
    : radius_(radius), speed_(speed), directionVector_(directionVector)
    {
        bodu_.setRadius(radius);
        bodu_.setOutlineColor(Color::Green); // Цвет линии обводки фигуры
        bodu_.setOutlineThickness(3);       // Толщина линии обводки фигуры
        bodu_.setFillColor(Color::Red);    // Цвет заливки фигуры
    }

    float getRadius()
    {
        return radius_;
    }

    void draw(RenderWindow& window)
    {
        window.draw(bodu_);
    }   

    void eat()
    {
        float newRadius = bodu_.getRadius() + 2; // Увеличиваем радиус шарика
        bodu_.setRadius(newRadius); //Увеличиваем радиус шарика
        bodu_.setOrigin({ newRadius, newRadius });
    }

    Vector2f getPosition()
    {
        return bodu_.getPosition();
    }

    CircleShape bodu_;
    float radius_;
    float speed_;
    Vector2f directionVector_;
private:    
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class Enemy : public Bass
{
public:
    Enemy()
    : Bass(10.f, 200.f, { 0.0f, 0.0f })
	{
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> distrib(0, 400);

        bodu_.setPosition({ static_cast<float>(distrib(gen)), static_cast<float>(distrib(gen)) });
	}

    void setDirection(Vector2f directionVector)
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

    void move(float deltaTime)
    {
        float frameSpeed = speed_ * deltaTime;
        Vector2f pos = directionVector_.normalized() * frameSpeed;
        bodu_.move(pos);
        directionVector_ = Vector2f(0.0f, 0.0f);
    }

    void moveCamera(Vector2f& directionVector)
    {
        bodu_.move(directionVector);
    }
    
private:    
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class Player : public Bass
{
public:
    Player()
    : Bass(20.f, 400.f, { 0.0f, 0.0f })
    {        
        bodu_.setPosition({ 400 - 20, 300 - 20 });
    }

    void setDirection(Vector2f directionVector)
    {
        directionVector_ = directionVector;
    }

    void move(float deltaTime)
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
            directionVector_ = Vector2f(0.0f, 0.0f);
        }
    }
    
private:
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML window");

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(1, 1000);
    uniform_real_distribution<float> distrib2(1.f, 1000.f);

    Player player;

    for (int i = 0; i < enemyCount; i++)
    {
        enemies.push_back(Enemy()); // Не понимаю немного...
    }

    for (int i = 0; i < puls; i++)
    {
        shsr.push_back(getShsr(distrib(gen), distrib(gen)));
    }

    Clock frameClock; //счетчик, чтобы считать время, за которое выводится кадр(frame), 1 сек / на это время = fps (кадры в сек)

    window.setFramerateLimit(60); //ограничим fps чтобы не было рывков в движении шариков 
    window.setVerticalSyncEnabled(true); //оптимизация тоже для увеличения плавности

    //Надпись с очками игрока
    const Font font("ARIAL.TTF");

    Text text(font, L"Oчки 0"); //L - чтоб были русские буквы вместо крякозябры.
    text.setCharacterSize(30); //Размер текста
    text.setStyle(Text::Bold);//Стиль текста
    text.setFillColor(Color::Blue);//Цвет текста

    Text gameWinText(font, L"Подзравляю, Вы выиграли Игру!"); //L - чтоб были русские буквы вместо крякозябры.
    gameWinText.setCharacterSize(40); //Размер текста
    gameWinText.setStyle(Text::Bold);//Стиль текста
    gameWinText.setFillColor(Color::Blue);//Цвет текста
    gameWinText.setPosition(sf::Vector2f(50.0f - 10, 300.0f - 60));


    // Начать игровой цикл
    while (window.isOpen())
    {
        // переменная в которой считаем время за которое выводится кадр
        // понятие deltaTime есть во многих движках, и везде оно обозначает одно и то же - время за которое выводится кадр
        float deltaTime = frameClock.getElapsedTime().asSeconds();

        std::cout << deltaTime << std::endl;
        cout << "fps: " << 1.f / deltaTime << "\n"; // просто для наглядности сколько сейчас у нас fps
        frameClock.restart(); // важно сбрасывать счетчик каждый кадр, чтобы считать именно время кадра

        // События процесса
        while (const std::optional event = window.pollEvent())
        {
            // Закрыть окно: выход
            if (event->is<sf::Event::Closed>())
                window.close();
            //Нажатие клавишь
            if (event->is<Event::KeyPressed>())
            {
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            Vector2i mousePosInt = sf::Mouse::getPosition(window);
            Vector2f mousePosFloat(static_cast<float>(mousePosInt.x), static_cast<float>(mousePosInt.y));
            Vector2f playerPos = player.Bass::getPosition();

            Vector2f newDirection = playerPos - mousePosFloat;

            player.setDirection(newDirection);
        }

        player.move(deltaTime);

        for (int i = 0; i < enemyCount; i++)
        {
            Vector2f posP = player.Bass::getPosition();
            Vector2f posE = enemies[i].Bass::getPosition();

            Vector2f posO = posP - posE;

            if (player.Bass::getRadius() > enemies[i].Bass::getRadius())
            {
                posO = -posO;
            }

            enemies[i].setDirection(posO);
            enemies[i].move(deltaTime);
        }

        //цикл определения столкновения с шариками
        for (int i = 0; i < shsr.size(); i++)
        {
            //определяем расстояние между нашим шариком и шариком из вектора
            //такой вектор можем получить просто отняв позицию шарика из вектора от позиции нашего нарика
            Vector2f rasoyanie = player.Bass::getPosition() - shsr[i].getPosition();
            //считаем сумму радиусов нашего шарика и шарика из вектора
            int sumRad = player.Bass::getRadius() + shsr[i].getRadius();
            //если расстояние между шариками меньше или равно сумме радиусов - значит шарики столкнулись
            if (rasoyanie.length() <= sumRad) // Сначала вписывал сюда && gameWin == false
            {
                cout << "Collision detected!" << endl;
                shsr[i].setPosition({ distrib2(gen), distrib2(gen) }); // Переходит на случайную позицию                

                player.Bass::eat(); // Увеличиваем размер игрока

                //Прибавляем очки и делаем условие победы/
                ochki++;
                if (ochki == 120)
                {
                    gameWin = true;
                }
                continue;
            }

            //Проверка столкновения с ботом
            for (int j = 0; j < enemies.size(); j++)
            {
                Vector2f conflict = enemies[j].Bass::getPosition() - shsr[i].getPosition();
                int summRadius = enemies[j].Bass::getRadius() + shsr[i].getRadius();

                if (conflict.length() <= summRadius)
                {
                    shsr[i].setPosition({ distrib2(gen), distrib2(gen) });
                    enemies[j].Bass::eat();
                }
            }

        }

        // Очистка окна.
        window.clear();

        //Рисуем вывод очков на доске
        text.setString(L"очки " + std::to_string(ochki));//Конвертируем int в string.(to_string)
        window.draw(text);

        for (int i = 0; i < shsr.size(); i++)
        {
            window.draw(shsr[i]);
        }

        player.Bass::draw(window);

        for (int i = 0; i < enemyCount; i++)
        {
            enemies[i].Bass::draw(window);
        }

        if (gameWin)
        {
            window.draw(gameWinText);
        }

        // Обновить окно
        window.display();
    }
}
//----------------------------------------------------------------------------------------------------------------------------------------------------------------