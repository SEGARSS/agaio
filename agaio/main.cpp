#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <random>

#include <vector>

using namespace std;
using namespace sf;

/*
������ ���� ����� 1000�1000
*/

int puls = 500;

//--------------------------------------------------------------------------------
CircleShape getShsr(float x, float y)//������� ���������� ������, � ������������� ��� ����������� ���������
{
    CircleShape shsr;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(0, 255);

    shsr.setRadius(5);                  // ������
    shsr.setOutlineColor(Color::Cyan); // ���� ����� ������� ������
    shsr.setOutlineThickness(1);      // ������� ����� ������� ������
    shsr.setFillColor( { static_cast<uint8_t>(distrib(gen)), static_cast<uint8_t>(distrib(gen)), static_cast<uint8_t>(distrib(gen)) }); // ���� ������� ������
    shsr.setPosition({ x, y}); 

    return shsr;
}
//---------------------------------------------------------------------------------------
enum class Direction//������������, ������ �������� ����������� ��������
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
    glShsr.setRadius(20);                  // ������
    glShsr.setOutlineColor(Color::Green); // ���� ����� ������� ������
    glShsr.setOutlineThickness(3);       // ������� ����� ������� ������
    glShsr.setFillColor(Color::Red);    // ���� ������� ������
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