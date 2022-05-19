#include "SFML/Graphics.hpp"
#include <iostream>

sf::Vector2f render(sf::Vector3f vector){
    return(sf::Vector2f(vector.x, vector.y));
}


int main()
{
    sf::RenderWindow window( sf::VideoMode( 600, 600 ), "SFML WORK!" );

    sf::RectangleShape line;

//    line.setSize(sf::Vector2f(20, 300));

    sf::Vertex line2[] =
    {
        sf::Vertex(sf::Vector2f(50, 10)),
        sf::Vertex(sf::Vector2f(200, 100))
    };


    while ( window.isOpen( ) )
    {
        sf::Event event;

        while ( window.pollEvent( event ) )
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();

                break;

            }
        }

        window.clear( );

        window.draw(line2, 2, sf::Lines);

        window.display( );
    }
}
