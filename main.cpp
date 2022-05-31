#include "SFML/Graphics.hpp"
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cmath>

float distance(sf::Vector2f P, float angle, sf::Vector2f _0)
 {
    return /*std::abs(*/(cos(angle*M_PI/180)*(P.x-_0.x))-(sin(angle*M_PI/180)*(_0.y-P.y))/*)*/;
 };

float perspective(sf::Vector2f point, sf::Vector2f cam, float angle, float fov)
 {
  if(distance(point,angle+90,cam)>0)
   {
    return(10*distance(cam,angle,point)/distance(point,angle+90,cam)*fov);
   }
  else
   {
    return 2137; // ooo panieeeeee
   };

 };

class Object2d
 {
  public:
    Object2d(sf::Vector2f position_, float angle_, float fov_)
     {
      position = position_;
      angle = angle_;
      fov = fov_;
     };
    sf::Vector2f pos2()
     {
      return sf::Vector2f((position.x-fov*sin(angle*M_PI/180)),(fov*cos(angle*M_PI/180)+position.y));
     };

    sf::Vector2f pos()
     {
      return position;
     };

    float ang()
     {
      return angle;
     };

    float gfov()
     {
      return fov;
     };

    void move(const sf::Time &elapsed)
     {
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
       {
        position.y -= 100*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
       {
        position.y += 100*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
       {
        position.x += 100*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
       {
        position.x -= 100*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
       {
        angle += 90*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
       {
        angle -= 90*elapsed.asSeconds();
       }
     }

  private:
   sf::Vector2f position;
   float angle;
   float fov;
 };

class Camera
 {
  public:
    Camera(sf::Vector3f position_, sf::Vector2f turn_, float fov_)
     {
      position = position_;
      turn = turn_;
      fov = fov_;
     };

    sf::Vector3f get_pos()
     {
      return position;
     };
    sf::Vector2f get_tur()
     {
      return turn;
     };
    float get_fov()
     {
      return fov;
     };

    void move(const sf::Time &elapsed)
     {
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
       {
        position.z += 100*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
       {
        position.z -= 100*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
       {
        position.x += 100*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
       {
        position.x -= 100*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
       {
        position.y -= 100*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
       {
        position.y += 100*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
       {
        fov += 50*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
       {
        fov -= 50*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
       {
        turn.x += 50*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
       {
        turn.x -= 50*elapsed.asSeconds();
       }
     }

  private:
   sf::Vector3f position;
   sf::Vector2f turn;
   float fov;
 };

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

    void set_position(int index, sf::Vector3f position)
     {
      Points[index]= position;
     };

    void print_point(int index)
     {
      std::cout<<Points[index].x<<std::endl;
     };


    void rotate(sf::Vector3f origin, float rv=M_PI/4, float time=1, int plane=1) // 1 = xy plane | 2 = yz plane | 3= xz plane
     {
       float orx, ory, px, py;


      if(plane==1) // XY
       {
        orx = origin.x;
        ory = origin.z;
       }
      else if(plane==2) // YZ
       {
        orx = origin.y;
        ory = origin.z;
       }
      else if(plane==3) // XZ
       {
        orx = origin.x;
        ory = origin.z;
       }

      for(auto &point : Points)
       {
        if(plane==1) // XY
         {
          px = point.x;
          py = point.y;
          point.x = orx+((px-orx)*cos(rv*time))+((ory-py)*sin(rv*time)); // Formulas i stole from the internet
          point.y = ory+((py-ory)*cos(rv*time))+((px-orx)*sin(rv*time));
         }
        else if(plane==2) // YZ
         {
          px = point.y;
          py = point.z;
          point.y = orx+((px-orx)*cos(rv*time))+((ory-py)*sin(rv*time)); // Formulas i stole from the internet
          point.z = ory+((py-ory)*cos(rv*time))+((px-orx)*sin(rv*time));
         }
        else if(plane==3) // XZ
         {
          px = point.x;
          py = point.z;
          point.x = orx+((px-orx)*cos(rv*time))+((ory-py)*sin(rv*time)); // Formulas i stole from the internet
          point.z = ory+((py-ory)*cos(rv*time))+((px-orx)*sin(rv*time));
         }

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



    sf::VertexArray p_render(Camera cam1, sf::Vector2u window) // p for perspective
     {
      sf::VertexArray array(type, vertexCount);
      float x,y;
      for (int i = 0; i < vertexCount; ++i)
       {
        x = perspective(sf::Vector2f(Points[i].x, Points[i].z), sf::Vector2f(cam1.get_pos().x, cam1.get_pos().z), cam1.get_tur().x, cam1.get_fov());
        y = perspective(sf::Vector2f(Points[i].y, Points[i].z), sf::Vector2f(cam1.get_pos().y, cam1.get_pos().z), cam1.get_tur().y, cam1.get_fov());
        array[i].position = sf::Vector2f(window.x/2-x*5, window.x/2-y*5);
       }
      return array;
     };



    int cube(std::vector<sf::Vector3f> points)
     {
      set_position(0, points[0]);
      set_position(1, points[1]);
      set_position(2, points[2]);
      set_position(3, points[3]);
      set_position(4, points[0]);
      set_position(5, points[4]);
      set_position(6, points[5]);
      set_position(7, points[1]);
      set_position(8, points[5]);
      set_position(9, points[6]);
      set_position(10, points[2]);
      set_position(11, points[6]);
      set_position(12, points[7]);
      set_position(13, points[3]);
      set_position(14, points[7]);
      set_position(15, points[4]);
     }


  private:
   std::size_t vertexCount;
   sf::PrimitiveType type;
   std::vector<sf::Vector3f> Points;
 };


int main()
{
    sf::RenderWindow window( sf::VideoMode( 800, 600 ), "SFML WORK!" );
    sf::Clock clock;

    Object3d coobe(sf::LineStrip,16);
    std::vector<sf::Vector3f> points {sf::Vector3f(200, 200, 200), sf::Vector3f(400, 200, 200), sf::Vector3f(400, 400, 200), sf::Vector3f(200, 400, 200),
                                      sf::Vector3f(200, 200, 400), sf::Vector3f(400, 200, 400), sf::Vector3f(400, 400, 400), sf::Vector3f(200, 400, 400)};
    coobe.cube(points);

    sf::Vector2f origin (400,300);

    sf::VertexArray graph(sf::LineStrip, 3);
    graph[0].position = sf::Vector2f(100,100);
    graph[1].position = sf::Vector2f(100,500);
    graph[2].position = sf::Vector2f(700,500);

//    coobe.rotate(sf::Vector3f(300,300,300),M_PI/4,1,1);
//    coobe.rotate(sf::Vector3f(300,300,300),M_PI/4,1,2);

    Camera cam01(sf::Vector3f(300,300,-500),sf::Vector2f(0,0), 30);

//    Object2d test1(sf::Vector2f(200,200), 0, 50);
//    sf::CircleShape dot(5);
//    sf::CircleShape dot2(5);
//    sf::CircleShape dot3(5);
//    dot2.setFillColor(sf::Color::Red);
//    dot3.setFillColor(sf::Color::Red);
//    dot2.setPosition(462, 281);
//    sf::VertexArray ray(sf::Lines, 2);
//    ray[0].color = sf::Color::Green;
//    ray[1].color = sf::Color::Green;


    while ( window.isOpen( ) )
    {
        sf::Time elapsed = clock.restart();
        sf::Event event;

//        test1.move(elapsed);
//        dot.setPosition(test1.pos().x-dot.getRadius(), test1.pos().y-dot.getRadius());
//        ray[0].position = sf::Vector2f(test1.pos());
//        ray[1].position = sf::Vector2f(test1.pos2());

//        float yee = perspective(dot2.getPosition(), test1.pos(), test1.ang(), test1.gfov());

//        dot3.setPosition(400+yee, 550);

//        std::cout<<yee<<" : "<<test1.ang()<<std::endl;

        cam01.move(elapsed);

        std::cout<<"X: "<<cam01.get_pos().x<<" Y: "<<cam01.get_pos().y<<" Z: "<<cam01.get_pos().z<<" Turn: "<<cam01.get_tur().x<<std::endl;

        while( window.pollEvent( event ) )
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();

                break;

            }
        }


//        coobe.rotate(sf::Vector3f(300,300,300),M_PI/4,elapsed.asSeconds(),3);

        window.clear( );

//        window.draw(coobe.render());
        window.draw(coobe.p_render(cam01, window.getSize()));
//        window.draw(graph);
//        window.draw(dot);
//        window.draw(dot2);
//        window.draw(dot3);
//        window.draw(ray);

        window.display( );
    }
}
