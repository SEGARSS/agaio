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
class Enemy
{
public:
    Enemy()
        : speed_(100.f), directionVector_(0.0f, 0.0f)
    {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> distrib(0, 400);

        enemies_.setRadius(10);                  // Радиус
        enemies_.setOutlineColor(Color::Green); // Цвет линии обводки фигуры
        enemies_.setOutlineThickness(3);       // Толщина линии обводки фигуры
        enemies_.setFillColor(Color::Red);    // Цвет заливки фигуры
        enemies_.setPosition({ static_cast<float>(distrib(gen)), static_cast<float>(distrib(gen)) });
    }

    void draw(RenderWindow& window)
    {
        window.draw(enemies_);
    }

    void moveCamera(Vector2f& directionVector)
    {
        enemies_.move(directionVector);
    }

    void move(float deltaTime)
    {
        float frameSpeed = speed_ * deltaTime;
        Vector2f pos = directionVector_.normalized() * frameSpeed;
        enemies_.move(pos);
        directionVector_ = Vector2f(0.0f, 0.0f);
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
			Vector2f closestSphere = shsr[0].getPosition() - enemies_.getPosition();

			for (int i = 1; i < shsr.size(); i++)
			{
				Vector2f posmin = shsr[i].getPosition() - enemies_.getPosition();

				//Vector2f posEn = enemies.getPosition();
				if (posmin.length() < closestSphere.length())
				{
					closestSphere = posmin;
				}				
			}
			directionVector_ = closestSphere;
		}
    }

    void eat()
    {
        float newRadius = enemies_.getRadius() + 2; // Увеличиваем радиус шарика
        enemies_.setRadius(newRadius); //Увеличиваем радиус шарика
        enemies_.setOrigin({ newRadius, newRadius });
    }

    float getRadius()
    {
        return enemies_.getRadius();
    }

    Vector2f getPosition()
    {
        return enemies_.getPosition();
    }

private:
    CircleShape enemies_;
    Vector2f directionVector_;
    float speed_; // скорость движения пикселей в секунду
};
//----------------------------------------------------------------------------------------------------------------------------------------------------------------
class Player
{
public:
    Player()
        : directionVector_(0.0f, 0.0f), speed_(400.f)
    {
        glShsr_.setRadius(20);                  // Радиус
        glShsr_.setOutlineColor(Color::Green); // Цвет линии обводки фигуры
        glShsr_.setOutlineThickness(3);       // Толщина линии обводки фигуры
        glShsr_.setFillColor(Color::Red);    // Цвет заливки фигуры
        glShsr_.setPosition({ 400 - 20, 300 - 20 });
    }

    void setDirection(Vector2f directionVector)
    {
        directionVector_ = directionVector;
    }

    Vector2f getPosition()
    {
        return glShsr_.getPosition();
    }

    void move(float deltaTime)
    {
        // тут у нас появляется понятие нормализации вектора
        // нормализация это приведение вектора к длине равной 1, но сохраняя направление вектора
        // делаем это чтобы наши шарики двигались с одинковой скоростью по диагонали
        // длина вектора считается как квадратный корень от суммы квадратов x и y
        // sqrt(x^2 + y^2) где sqrt - квадратный корень, ^2 - возведение в степень 2 или в квадрат
        // на пример x = 1, y = 0 => sqrt(1^2 + 0^2) = sqrt(1 + 0) = sqrt(1) = 1
        // на пример x = 1, y = 1 => sqrt(1^2 + 1^2) = sqrt(1 + 1) = sqrt(2) = 1.41 , т.е. почти на 40% быстрее двигаться по диагонали
        // после нормализации получаем x = 0.71 y = 0.71, и длина вектора примерно 1
        if (directionVector_ != Vector2f(0.0f, 0.0f) && gameWin == false)
        {
            //считаем на сколько нужно сдвинуть шарики, учитывая время кадра
            //если скорость 400 пикселей в сек, а кадр выводится за 0,0004 секунды, 
            //значит нам нужно переместить в текущем кадре шарики всего на 1,6 пикселя, и тогда за 1 секунду мы переместимся на 400 пикселей
            float frameSpeed = speed_ * deltaTime;
            // { 1, 0 } -> { 1, 0 }
            // { 1, 1 } -> { 0.71, 0.71 }
            //// { 0.24, 0 }
            // нормализируем вектор направления и только после этого умножаем на скорость в текущем кадре
            directionVector_ = directionVector_.normalized() * frameSpeed;
            for (int i = 0; i < shsr.size(); i++)
            {
                shsr[i].move(directionVector_);
            }

            for (int i = 0; i < enemies.size(); i++)
            {
                enemies[i].moveCamera(directionVector_);
            }
            directionVector_ = Vector2f(0.0f, 0.0f);// не забываем сбросить вектор направления, иначе бует двигаться даже после отпускания кнопки движения


        }
    }

    void eat()
    {
        float newRadius = glShsr_.getRadius() + 5; // Увеличиваем радиус шарика
        glShsr_.setRadius(newRadius); //Увеличиваем радиус шарика
        glShsr_.setOrigin({ newRadius, newRadius }); // Устанавливаем центр фигуры в центр круга, чтобы при перемещении он двигался от центраq
    }

    float getRadius()
    {
        return glShsr_.getRadius();
    }

    void draw(RenderWindow& window)
    {
        window.draw(glShsr_);
    }

private:
    CircleShape glShsr_;
    Vector2f directionVector_; // вектор направления, сделали взамен enum direction
    float speed_; // скорость движения пикселей в секунду
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
        enemies.push_back(Enemy());
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
            Vector2f playerPos = player.getPosition();

            Vector2f newDirection = playerPos - mousePosFloat;

            player.setDirection(newDirection);
        }

        player.move(deltaTime);

        for (int i = 0; i < enemyCount; i++)
        {
            Vector2f posP = player.getPosition();
            Vector2f posE = enemies[i].getPosition();

            Vector2f posO = posP - posE;

            if (player.getRadius() > enemies[i].getRadius())
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
            Vector2f rasoyanie = player.getPosition() - shsr[i].getPosition();
            //считаем сумму радиусов нашего шарика и шарика из вектора
            int sumRad = player.getRadius() + shsr[i].getRadius();
            //если расстояние между шариками меньше или равно сумме радиусов - значит шарики столкнулись
            if (rasoyanie.length() <= sumRad) // Сначала вписывал сюда && gameWin == false
            {
                cout << "Collision detected!" << endl;
                shsr[i].setPosition({ distrib2(gen), distrib2(gen) }); // Переходит на случайную позицию                

                player.eat(); // Увеличиваем размер игрока

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
                Vector2f conflict = enemies[j].getPosition() - shsr[i].getPosition();
                int summRadius = enemies[j].getRadius() + shsr[i].getRadius();

                if (conflict.length() <= summRadius)
                {
                    shsr[i].setPosition({ distrib2(gen), distrib2(gen) });
                    enemies[j].eat();
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

        player.draw(window);

        for (int i = 0; i < enemyCount; i++)
        {
            enemies[i].draw(window);
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