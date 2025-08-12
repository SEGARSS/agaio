#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <iostream>
#include <chrono>
#include <vector>
#include <string>

#include "Enemy.h"
#include "Player.h"

using namespace std;
using namespace sf;

namespace Game {
    enum class State {
        NameInput,
        Play,
        GameEnd,
    };
};


Game::State currentState = Game::State::NameInput;
std::string playerName;

//---------------------------------------------------------------------------------------------------------------
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


//---------------------------------------------------------------------------------------------------------------
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
//---------------------------------------------------------------------------------------------------------------
void generateWorld() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 1000);

    shsr.clear();
    for (int i = 0; i < puls; i++)
    {
        shsr.push_back(getShsr(distrib(gen), distrib(gen)));
    }
}


//---------------------------------------------------------------------------------------------------------------
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
        Enemy enemy;
        enemies.push_back(enemy);
        //enemies.push_back(Enemy()); // Вторйо вариант объявления
    }

    generateWorld();

    Clock frameClock; //счетчик, чтобы считать время, за которое выводится кадр(frame), 1 сек / на это время = fps (кадры в сек)

    Clock clockEnemyShoot; // Создаём часы (начало таймера)
    chrono::milliseconds tickEnemyShoot(1500); // После какого времени таймер начал делать всё сначала или сброс.

    Clock clock;

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

    Text gameOverText(font, L"Ты лузер, проиграл!"); //L - чтоб были русские буквы вместо крякозябры.
    gameOverText.setCharacterSize(60); //Размер текста
    gameOverText.setStyle(Text::Bold);//Стиль текста
    gameOverText.setFillColor(Color::Red);//Цвет текста
    gameOverText.setPosition(sf::Vector2f(120.0f - 60, 300.0f - 60));

    bool gameOver = false;

    // Начать игровой цикл
    while (window.isOpen())
    {
        switch (currentState)
        {
        case Game::State::NameInput:
        {
            // События процесса
            while (const std::optional event = window.pollEvent())
            {
                // Закрыть окно: выход
                if (event->is<sf::Event::Closed>())
                    window.close();

                //Ввод текста
                if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
                {
                    if (textEntered->unicode < 128) {
                        if (textEntered->unicode == '\b' && playerName != "")
                        {
                            playerName.pop_back();
                        }
                        else {
                            playerName += static_cast<char>(textEntered->unicode);
                        }
                    }
                    std::cout << playerName << std::endl;
                }
            }

            /// <-----


            break;
        }
        case Game::State::Play:
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
            }

            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                Vector2i mousePosInt = sf::Mouse::getPosition(window);
                Vector2f mousePosFloat(static_cast<float>(mousePosInt.x), static_cast<float>(mousePosInt.y));
                Vector2f playerPos = player.Base::getPosition();

                Vector2f newDirection = playerPos - mousePosFloat;

                player.setDirection(newDirection);
            }

            player.move(deltaTime);

            for (int i = 0; i < enemies.size(); i++)
            {
                Vector2f posP = player.Base::getPosition();
                Vector2f posE = enemies[i].Base::getPosition();

                Vector2f posO = posP - posE;

                if (player.Base::getRadius() >= enemies[i].Base::getRadius())
                {
                    posO = -posO;
                }

                enemies[i].setDirection(posO);
                enemies[i].move(deltaTime);
            }

            //цикл определения столкновения с врагами
            for (int i = 0; i < enemies.size(); ++i)
            {
                //определяем расстояние между нашим шариком и шариком из вектора
                //такой вектор можем получить просто отняв позицию шарика из вектора от позиции нашего нарика
                Vector2f rasoyanie = player.Base::getPosition() - enemies[i].getPosition();
                //считаем сумму радиусов нашего шарика и шарика из вектора
                int sumRad = player.Base::getRadius() + enemies[i].getRadius();
                //если расстояние между шариками меньше или равно сумме радиусов - значит шарики столкнулись
                if (rasoyanie.length() <= sumRad) // Сначала вписывал сюда && gameWin == false
                {
                    if (player.Base::getRadius() > enemies[i].Base::getRadius())
                    {
                        player.Base::eat(enemies[i].getRadius() / 2); // Увеличиваем размер игрока
                        ochki += enemies[i].getRadius() / 2;
                        enemies.erase(enemies.begin() + i); // Удаляем врага из вектора

                        //Прибавляем очки и делаем условие победы/
                        if (ochki == 120)
                        {
                            gameWin = true;
                            currentState = Game::State::GameEnd;
                        }
                        continue;
                    }
                    else
                    {
                        enemies.clear();
                        gameOver = true;
                        currentState = Game::State::GameEnd;
                    }
                }
            }


            //цикл определения столкновения с шариками
            for (int i = 0; i < shsr.size(); i++)
            {
                //определяем расстояние между нашим шариком и шариком из вектора
                //такой вектор можем получить просто отняв позицию шарика из вектора от позиции нашего нарика
                Vector2f rasoyanie = player.Base::getPosition() - shsr[i].getPosition();
                //считаем сумму радиусов нашего шарика и шарика из вектора
                int sumRad = player.Base::getRadius() + shsr[i].getRadius();
                //если расстояние между шариками меньше или равно сумме радиусов - значит шарики столкнулись
                if (rasoyanie.length() <= sumRad) // Сначала вписывал сюда && gameWin == false
                {
                    cout << "Collision detected!" << endl;
                    shsr[i].setPosition({ distrib2(gen), distrib2(gen) }); // Переходит на случайную позицию                

                    player.Base::eat(); // Увеличиваем размер игрока

                    //Прибавляем очки и делаем условие победы/
                    ochki++;
                    if (ochki == 120)
                    {
                        gameWin = true;
                        currentState = Game::State::GameEnd;
                    }
                    continue;
                }

                //Проверка столкновения с ботом
                for (int j = 0; j < enemies.size(); j++)
                {
                    Vector2f conflict = enemies[j].Base::getPosition() - shsr[i].getPosition();
                    int summRadius = enemies[j].Base::getRadius() + shsr[i].getRadius();

                    if (conflict.length() <= summRadius)
                    {
                        shsr[i].setPosition({ distrib2(gen), distrib2(gen) });
                        enemies[j].Base::eat();
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

            for (int i = 0; i < enemies.size(); i++)
            {
                enemies[i].Base::draw(window);
            }

            player.Base::draw(window);
            break;
        }
        case Game::State::GameEnd: {
            // События процесса
            while (const std::optional event = window.pollEvent())
            {
                // Закрыть окно: выход
                if (event->is<sf::Event::Closed>())
                    window.close();
                //Нажатие клавишь
                if (event->is<Event::KeyPressed>())
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
                    {
                        if (gameOver == true || gameWin == true)
                        {
                            for (int i = 0; i < enemyCount; i++)
                            {
                                enemies.push_back(Enemy());
                            }
                            ochki = 0;
                            gameOver = false;
                            gameWin = false;
                            player = Player();
                            generateWorld();
                            currentState = Game::State::Play;
                            //player.resetRadius();
                        }
                    }
                }
            }

            // Очистка окна.
            window.clear();

            if (gameWin)
            {
                window.draw(gameWinText);
            }

            if (gameOver)
            {
                window.draw(gameOverText);
            }

            break;
        }
        default:
            break;
        }

        // Обновить окно
        window.display();
    }
}
//---------------------------------------------------------------------------------------------------------------