#include <iostream>
#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");
    window.setFramerateLimit(120);

    sf::Texture grass_t;
    if (!grass_t.loadFromFile("grass.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }
    sf::Texture guy_t;
    if (!guy_t.loadFromFile("guy.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }
    sf::Texture wall_t;
    if (!wall_t.loadFromFile("wall.png")) {
        std::cerr << "Could not load texture" << std::endl;
        return 1;
    }

    sf::Sprite sprite;
    sprite.setTexture(grass_t);
    sprite.setTextureRect(sf::IntRect(0,0,800,600)); //left, top, width, height
    grass_t.setRepeated(true);

    sf::Sprite sprite2;
    sprite2.setTexture(guy_t);


    sf::Sprite sprite3;
    sprite3.setTexture(wall_t);
    sprite3.setTextureRect(sf::IntRect(50,50,60,190));
    wall_t.setRepeated(true);
    sprite3.setPosition(40,80);

    sf::Sprite sprite4;
    sprite4.setTexture(wall_t);
    sprite4.setTextureRect(sf::IntRect(50,50,60,390));
    wall_t.setRepeated(true);
    sprite4.setPosition(220,140);

    sf::Sprite sprite5;
    sprite5.setTexture(wall_t);
    sprite5.setTextureRect(sf::IntRect(50,50,160,60));
    wall_t.setRepeated(true);
    sprite5.setPosition(340,70);

    sf::Sprite sprite6;
    sprite6.setTexture(wall_t);
    sprite6.setTextureRect(sf::IntRect(50,50,60,390));
    wall_t.setRepeated(true);
    sprite6.setPosition(550,120);

    sf::Sprite sprite7;
    sprite7.setTexture(wall_t);
    sprite7.setTextureRect(sf::IntRect(50,50,160,60));
    wall_t.setRepeated(true);
    sprite7.setPosition(550,430);

std::vector<sf::Sprite> walls = {sprite3, sprite4, sprite5, sprite6, sprite7};


//collison
sf::FloatRect nextPos;

//const float gridSize = 50.f;
    sf::Clock clock;
    // run the program as long as the window is open
    while (window.isOpen()) {

        sf::Time elapsed = clock.restart();
        float dt = elapsed.asSeconds();
        int x_speed = 100;
        int y_speed = 150;

        for(auto &wall: walls){
            sf::FloatRect playerBounds = sprite2.getGlobalBounds();
            sf::FloatRect wallBounds = wall.getGlobalBounds();
            nextPos = playerBounds;
            nextPos.left += x_speed;
            nextPos.top += y_speed;

            if(wallBounds.intersects(nextPos)){
                std::cout<<"collision detected"<<"\n";
                if(playerBounds.top < wallBounds.top
                   && playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
                   && playerBounds.left < wallBounds.left + wallBounds.width
                   && playerBounds.left + playerBounds.width > wallBounds.left){
                    std::cout<<"bcolllision";
                    sprite2.setPosition(playerBounds.left,wallBounds.top-playerBounds.height);
                }
                else if( playerBounds.top > wallBounds.top
                        && playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
                        && playerBounds.left < wallBounds.left + wallBounds.width
                        && playerBounds.left + playerBounds.width > wallBounds.left){
                    std::cout<<"tcolllision";
                    sprite2.setPosition(playerBounds.left,wallBounds.top-playerBounds.height);
                }
                else if( playerBounds.left < wallBounds.left
                        && playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
                        && playerBounds.top < wallBounds.top + wallBounds.height
                        && playerBounds.top + playerBounds.height > wallBounds.top){
                    std::cout<<"rcolllision";
                    sprite2.setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
                }
                else if( playerBounds.left > wallBounds.left
                        && playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
                        && playerBounds.top < wallBounds.top + wallBounds.height
                        && playerBounds.top + playerBounds.height > wallBounds.top){
                    std::cout<<"lcolllision";
                    sprite2.setPosition(wallBounds.left - wallBounds.width, playerBounds.top);
                }
            }
}





 sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            sprite2.move(0, -abs(y_speed)*dt);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
            sprite2.move(0, abs(y_speed)*dt);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
            sprite2.move(abs(x_speed)*dt,0);
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
            sprite2.move(-abs(x_speed)*dt,0);
        }


        if(sprite2.getPosition().x < 0.f){
            sprite2.setPosition(0.f, sprite2.getPosition().y);
        }
        else if(sprite2.getPosition().y < 0.f){
            sprite2.setPosition(sprite2.getPosition().x, 0.f);
        }
        else if(sprite2.getPosition().x + sprite2.getGlobalBounds().width > 800){

            sprite2.setPosition(800-sprite2.getGlobalBounds().width, sprite2.getPosition().y);
        }
        else if(sprite2.getPosition().y + sprite2.getGlobalBounds().height > 600){

            sprite2.setPosition(sprite2.getPosition().x,600-sprite2.getGlobalBounds().height);
        }

        // clear the window with black color
        window.clear(sf::Color::Black);
        // draw everything here...
        window.draw(sprite);//trawa
        for(auto &wall: walls){
            window.draw(wall);
        }


        window.draw(sprite2);
        window.display();
    }

    return 0;
}
