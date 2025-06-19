#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>

#include <vector>

using namespace std;
using namespace sf;

/*
Размер мира будет 1000х1000
*/

int puls = 500;

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

    Direction direction = Direction::stop;


    // Start the game loop
    while (window.isOpen())
    {
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
                    direction = Direction::down;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                {
                    direction = Direction::right;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                {
                    direction = Direction::left;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                {
                    direction = Direction::up;
                }
            }
        }

        for (int i = 0; i < shsr.size(); i++)
        {
            if (direction == Direction::down)
            {
                shsr[i].move(Vector2f(0.0, -3.0));
            }
            if(direction == Direction::right)
            {
                shsr[i].move(Vector2f(-3.0, 0.0));
            }
            if (direction == Direction::left)
            {
                shsr[i].move(Vector2f(+3.0, 0.0));
            }
            if (direction == Direction::up)
            {
                shsr[i].move(Vector2f(0.0, +3.0));
            }
        }
        direction = Direction::stop;

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