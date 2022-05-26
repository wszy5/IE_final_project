#include "SFML/Graphics.hpp"
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cmath>

class Object3d : sf::VertexArray
 {
  public:
    Object3d(sf::PrimitiveType type_, std::size_t vertexCount_ = 0) : sf::VertexArray(type_, vertexCount_) // constructor
     {
      type = type_;
      vertexCount = vertexCount_;
       for (int i=0; i<vertexCount_; ++i)
        {
         Points.push_back(sf::Vector3f(0,0,0));
        }
     }
//    Object3d() : sf::VertexArray(){};

//    sf::Vector3f& operator [](std::size_t index);
//    const sf::Vector3f& operator [](std::size_t index) const;

    void set_position(int index, sf::Vector3f position)
     {
      Points[index]= position;
     };

    void print_point(int index)
     {
      std::cout<<Points[index].x<<std::endl;
     };

    void rotate(sf::Vector2f origin, float rv = M_PI/4, float time =0)
     {
      for(auto &point : Points)
       {
        point.x = origin.x+((point.x-origin.x)*cos(rv*time))+((origin.y-point.z)*sin(rv*time)); // Formulas i stole from the internet
        point.z = origin.y+((point.z-origin.y)*cos(rv*time))+((point.x-origin.x)*sin(rv*time));
       }
     }

    sf::VertexArray render()
     {
      sf::VertexArray array(type, vertexCount);
      for (int i = 0; i < vertexCount; ++i)
       {
        array[i].position = sf::Vector2f(Points[i].x, Points[i].y); // this is just that code retrofited into a class function
       }
      return array;
     };



  private:
   std::size_t vertexCount;
   sf::PrimitiveType type;
   std::vector<sf::Vector3f> Points;
 };


//sf::Vector2f render(sf::Vector3f vector){
//    return(sf::Vector2f(vector.x, vector.y));
//}


int main()
{
    sf::RenderWindow window( sf::VideoMode( 800, 600 ), "SFML WORK!" );
    sf::Clock clock;

    Object3d coobe(sf::LineStrip,16);

    sf::Vector2f origin (400,300);

    sf::CircleShape dot(10);
    dot.setPosition (600, 300);

    coobe.set_position(0, sf::Vector3f(100, 100, 100)); // Because of how Primitive types work in SFML we have to initialise
    coobe.set_position(1, sf::Vector3f(500, 100, 100)); // a couple more points than is actually present in the structure
    coobe.set_position(2, sf::Vector3f(530, 70, 500)); // We should try to find a way to fix/automate/streamline this
    coobe.set_position(3, sf::Vector3f(70, 70, 500));
    coobe.set_position(4, sf::Vector3f(100, 100, 100));
    coobe.set_position(5, sf::Vector3f(100, 500, 100));
    coobe.set_position(6, sf::Vector3f(500, 500, 100));
    coobe.set_position(7, sf::Vector3f(500, 100, 100));
    coobe.set_position(8, sf::Vector3f(500, 500, 100));
    coobe.set_position(9, sf::Vector3f(530, 530, 500));
    coobe.set_position(10, sf::Vector3f(530, 70, 500));
    coobe.set_position(11, sf::Vector3f(530, 530, 500));
    coobe.set_position(12, sf::Vector3f(70, 530, 500));
    coobe.set_position(13, sf::Vector3f(70, 70, 500));
    coobe.set_position(14, sf::Vector3f(70, 530, 500));
    coobe.set_position(15, sf::Vector3f(100, 500, 100));



//    sf::Vertex point;

//    point.position = sf::Vector2f(100, 100);
//    point.color = sf::Color::Red;

//    sf::VertexArray line(sf::Lines, 2);

//    line[0].position = sf::Vector2f(150, 150);
//    line[0].color = sf::Color::Blue;
//    line[1].position = sf::Vector2f(180, 180);
//    line[1].color = sf::Color::Yellow;

//   line[1] = point;

//   sf::VertexArray tri(sf::Triangles, 3);

//   tri[0].position = sf::Vector2f(222, 224);
//   tri[0].color = sf::Color::Blue;
//   tri[1].position = sf::Vector2f(289, 111);
//   tri[1].color = sf::Color::Red;
//   tri[2].position = sf::Vector2f(400, 299);
//   tri[2].color = sf::Color::Green;


//    sf::RectangleShape line;
//    line.setSize(sf::Vector2f(20, 300));
//    sf::Vertex line2[] =
//    {
//        sf::Vertex(sf::Vector2f(50, 10)),
//        sf::Vertex(sf::Vector2f(200, 100))
//    };
    //test


    while ( window.isOpen( ) )
    {
        sf::Time elapsed = clock.restart();

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


        coobe.rotate(sf::Vector2f(300,300),M_PI/4,elapsed.asSeconds());

        window.clear( );

        window.draw(coobe.render());
//        window.draw(tri);

        window.display( );
    }
}
