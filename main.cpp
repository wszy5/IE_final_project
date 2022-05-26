#include "SFML/Graphics.hpp"
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>

class Object3d : sf::VertexArray
 {
  public:
    Object3d(sf::PrimitiveType type_, std::size_t vertexCount_ = 0) : sf::VertexArray(type_, vertexCount_)
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

    sf::VertexArray render()
     {
      sf::VertexArray array(type, vertexCount);
      for (int i = 0; i < vertexCount; ++i)
       {
        array[i].position = sf::Vector2f(Points[i].x, Points[i].y);
       }
      return array;
     };

  private:
   std::size_t vertexCount;
   sf::PrimitiveType type;
   std::vector<sf::Vector3f> Points;
 };


sf::Vector2f render(sf::Vector3f vector){
    return(sf::Vector2f(vector.x, vector.y));
}


int main()
{
    sf::RenderWindow window( sf::VideoMode( 800, 600 ), "SFML WORK!" );

    Object3d coobe(sf::LineStrip,16);

//    coobe[3] = sf::Vector3f(100, 100, 100);
    coobe.set_position(0, sf::Vector3f(100, 100, 100));
    coobe.set_position(1, sf::Vector3f(500, 100, 100));
    coobe.set_position(2, sf::Vector3f(510, 90, 500));
    coobe.set_position(3, sf::Vector3f(90, 90, 500));
    coobe.set_position(4, sf::Vector3f(100, 100, 100));
    coobe.set_position(5, sf::Vector3f(100, 500, 100));
    coobe.set_position(6, sf::Vector3f(500, 500, 100));
    coobe.set_position(7, sf::Vector3f(500, 100, 100));
    coobe.set_position(8, sf::Vector3f(500, 500, 100));
    coobe.set_position(9, sf::Vector3f(510, 510, 500));
    coobe.set_position(10, sf::Vector3f(510, 90, 500));
    coobe.set_position(11, sf::Vector3f(510, 510, 500));
    coobe.set_position(12, sf::Vector3f(90, 510, 500));
    coobe.set_position(13, sf::Vector3f(90, 90, 500));
    coobe.set_position(14, sf::Vector3f(90, 510, 500));
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

        window.draw(coobe.render());
//        window.draw(tri);

        window.display( );
    }
}
