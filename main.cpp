#include "SFML/Graphics.hpp"
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include<windows.h>


float distance(sf::Vector2f P, float angle, sf::Vector2f _0)
 {
  return /*std::abs(*/(cos(angle*M_PI/180)*(P.x-_0.x))-(sin(angle*M_PI/180)*(_0.y-P.y))/*)*/;
 };

float distance_3d(sf::Vector3f p1, sf::Vector3f p2)
 {
  return sqrtf(powf(p2.x-p1.x,2)+powf(p2.y-p1.y,2)+powf(p2.z-p1.z,2));
 };

sf::Vector2f perspective(sf::Vector3f point, sf::Vector3f cam, float angle, float fov, sf::Vector2u window)
 {
  float X,Y;
  if(distance(sf::Vector2f(point.x,point.z),angle+90,sf::Vector2f(cam.x,cam.z))>1)
   {
    X=(window.x/2)-(10*distance(sf::Vector2f(cam.x,cam.z),angle,sf::Vector2f(point.x,point.z))/distance(sf::Vector2f(point.x,point.z),angle+90,sf::Vector2f(cam.x,cam.z))*fov); // K/M*fov
    Y=(window.y/2)-(10*(cam.y-point.y)/distance(sf::Vector2f(point.x,point.z),angle+90,sf::Vector2f(cam.x,cam.z))*fov);
   }
  else
   {
    X=(window.x/2)-(10*distance(sf::Vector2f(cam.x,cam.z),angle,sf::Vector2f(point.x,point.z))*fov);
    Y=(window.y/2)-(10*(cam.y-point.y)*fov);
   };

  return(sf::Vector2f(X,Y));
 };

class Object2d //debug feature to deal with pesky stuff like motion
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
      return sf::Vector2f((position.x-fov*sin(angle*M_PI/180)),(position.y-fov*cos(angle*M_PI/180)));
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
//      std::cout<<"Angle: "<<angle<<" cos(angle): "<<cos(angle*M_PI/180)<<" sin(angle): "<<sin(angle*M_PI/180)<<std::endl;

      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
       {
        position.y -= 100*elapsed.asSeconds()*cos(angle*M_PI/180);
        position.x -= 100*elapsed.asSeconds()*sin(angle*M_PI/180);
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
    Camera(sf::Vector3f position_, float turn_, float fov_)
     {
      position = position_;
      turn = turn_;
      fov = fov_;
      state = 0;
      has_exited=false;
      CoM_y = position.y;
      Ani_y = 0;
      minicamera.setFillColor(sf::Color::Yellow);
      minicamera.setRadius(20);
      minicamera.setPosition(sf::Vector2f(position.x,-position.z));
     };

    sf::Vector3f get_pos()
     {
      return position;
     };
    float get_tur()
     {
      return turn;
     };
    float get_fov()
     {
      return fov;
     };
    int get_state()
     {
      return state;
     };
    bool get_exited()
     {
      return has_exited;
     };
    void set_state(int nstate)
     {
      state = nstate;
     };
    sf::CircleShape get_mini()
     {
      return minicamera;
     }
    void move(const sf::Time &elapsed, const sf::Window &window, const sf::Time &elapsed_w, std::vector<sf::RectangleShape> walls, std::vector<sf::RectangleShape> doors)
     {

      int offset = sf::Mouse::getPosition(window).x-window.getSize().x/2; //Turning the camera with the mouse movement
      turn -= 8*elapsed.asSeconds()*offset;
      sf::Mouse::setPosition(sf::Vector2i(window.getSize().x/2, window.getSize().y/2), window);

      if(state==1)
       {
        Ani_y = (cos(12*elapsed_w.asSeconds())-1)*5/3; //running animation (camera bobbing up and down)
       }
      else if(state==2)
       {
        Ani_y -=elapsed.asSeconds()/12;
        if(Ani_y < 0)
         {
          Ani_y=0;
          state=0;
         }
       }

      velocity.y = 0.f;
      velocity.x = 0.f;
      velocity.z = 0.f;

      if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
       {
        velocity.z += movementSpeed*elapsed.asSeconds()*cos(turn*M_PI/180);
        velocity.x -= movementSpeed*elapsed.asSeconds()*sin(turn*M_PI/180);
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
       {
        velocity.z -= movementSpeed*elapsed.asSeconds()*cos(turn*M_PI/180);
        velocity.x += movementSpeed*elapsed.asSeconds()*sin(turn*M_PI/180);
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
       {
        velocity.z += movementSpeed*elapsed.asSeconds()*sin(turn*M_PI/180);
        velocity.x += movementSpeed*elapsed.asSeconds()*cos(turn*M_PI/180);
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
       {
        velocity.z -= movementSpeed*elapsed.asSeconds()*sin(turn*M_PI/180);
        velocity.x -= movementSpeed*elapsed.asSeconds()*cos(turn*M_PI/180);
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
       {
        CoM_y -= movementSpeed*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
       {
        CoM_y += movementSpeed*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::O))
       {
        fov += movementSpeed/2*elapsed.asSeconds();
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::P))
       {
        fov -= movementSpeed/2*elapsed.asSeconds();
       }
      position.y = CoM_y + Ani_y;


      //Collision
      for (auto &wall : walls)
      {
          sf::FloatRect playerBounds = minicamera.getGlobalBounds();
          sf::FloatRect wallBounds = wall.getGlobalBounds();

          nextPos = playerBounds;
          nextPos.left += velocity.x;
          nextPos.top -= velocity.z;

          if (wallBounds.intersects(nextPos))
          {
              //Bottom collision
              if (playerBounds.top < wallBounds.top
                  && playerBounds.top + playerBounds.height < wallBounds.top + wallBounds.height
                  && playerBounds.left < wallBounds.left + wallBounds.width
                  && playerBounds.left + playerBounds.width > wallBounds.left
                  )
              {
                  velocity.z = 0.f;
                  minicamera.setPosition(playerBounds.left, wallBounds.top - playerBounds.height);
              }

              //Top collision
              else if (playerBounds.top > wallBounds.top
                  && playerBounds.top + playerBounds.height > wallBounds.top + wallBounds.height
                  && playerBounds.left < wallBounds.left + wallBounds.width
                  && playerBounds.left + playerBounds.width > wallBounds.left
                  )
              {
                  velocity.z = 0.f;
                  minicamera.setPosition(playerBounds.left, wallBounds.top + wallBounds.height);
              }

              //Right collision
              if (playerBounds.left < wallBounds.left
                  && playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
                  && playerBounds.top < wallBounds.top + wallBounds.height
                  && playerBounds.top + playerBounds.height > wallBounds.top
                  )
              {
                  velocity.x = 0.f;
                  minicamera.setPosition(wallBounds.left - playerBounds.width, playerBounds.top);
              }

              //Left collision
              else if (playerBounds.left > wallBounds.left
                  && playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
                  && playerBounds.top < wallBounds.top + wallBounds.height
                  && playerBounds.top + playerBounds.height > wallBounds.top
                  )
              {
                  velocity.x = 0.f;
                  minicamera.setPosition(wallBounds.left+ + wallBounds.width, playerBounds.top);
              }
          }
      }

      for (auto &wall : doors)
      {
          sf::FloatRect playerBounds = minicamera.getGlobalBounds();
          sf::FloatRect wallBounds = wall.getGlobalBounds();

          nextPos1 = playerBounds;
          nextPos1.left += velocity.x;
          nextPos1.top -= velocity.z;

          if (wallBounds.intersects(nextPos1))
          {
           if(1) //idk
            {
             has_exited=true;
             std::cout<<"pp: "<<has_exited<<std::endl;
            }
          }
      }

      position.x += velocity.x;
      position.y += velocity.y;
      position.z += velocity.z;

      minicamera.setPosition(sf::Vector2f(position.x+(scale/2)-(minicamera.getRadius()), -position.z+(scale/2)-(minicamera.getRadius())));
     }

  private:
   sf::Vector3f position;
   float scale = 100;
   float turn;
   float fov;
   bool has_exited;
   float CoM_y; //position y with disregrard for animation
   float Ani_y; //position y with regard only to animation
   int state; // state being a state of animation 0-standing 1-walking 2-stopping
   float movementSpeed = 50;
   sf::Vector3f velocity;
   sf::CircleShape minicamera;
   sf::FloatRect nextPos, nextPos1;

 };

class Object3d : sf::VertexArray
 {
  public:
    Object3d(sf::PrimitiveType type_, std::size_t vertexCount_ = 0, sf::Color color_ = sf::Color::White) : sf::VertexArray(type_, vertexCount_) // constructor
     {
      type = type_;
      vertexCount = vertexCount_;
      color = color_;
       for (int i=0; i<vertexCount_; ++i)
        {
         Points.push_back(sf::Vector3f(0,0,0));
        }
     }

    void set_position(int index, sf::Vector3f position)
     {
      Points[index]= position;
      float x=0,y=0,z=0;
      for(auto point : Points)
       {
        x+=point.x;
        y+=point.y;
        z+=point.z;
       }
      CoM = sf::Vector3f(x/Points.size(),y/Points.size(),z/Points.size());
     };

    void print_point(int index)
     {
      std::cout<<Points[index].x<<std::endl;
     };

    sf::Vector3f get_com()
     {
      return CoM;
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
      sf::VertexArray null(sf::Points, 1);

      bool abort = 1;
      for (int i = 0; i < vertexCount; ++i)
       {
        if(distance(sf::Vector2f(Points[i].x,Points[i].z),cam1.get_tur()+90,sf::Vector2f(cam1.get_pos().x,cam1.get_pos().z))>0)
         {
          abort=0;
          break;
         }
       }
      if(abort==0)
       {
        for (int i = 0; i < vertexCount; ++i)
         {
          array[i].position = perspective(Points[i],cam1.get_pos(),cam1.get_tur(),cam1.get_fov(),window);
          array[i].color = color;
         }
        return array;
       }
      else
       {
        return null;
       }
     };




    void cube(std::vector<sf::Vector3f> points)
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
   sf::Color color;
   sf::Vector3f CoM; // centre of mass
   std::vector<sf::Vector3f> Points;
 };

std::vector<Object3d> construct_panel(sf::Vector2f point1, sf::Vector2f point2, sf::Vector2f height, int type, sf::Color color = sf::Color::Blue)
 {
  std::vector<Object3d> panel;
  Object3d fill(sf::Quads, 4, color);
  fill.set_position(0, sf::Vector3f(point1.x,height.y,point1.y));
  fill.set_position(1, sf::Vector3f(point2.x,height.y,point2.y));
  fill.set_position(2, sf::Vector3f(point2.x,height.x,point2.y));
  fill.set_position(3, sf::Vector3f(point1.x,height.x,point1.y));
  panel.emplace_back(fill);
  if(type == 1)
   {
    Object3d border(sf::LineStrip, 5);
    border.set_position(0, sf::Vector3f(point1.x,height.y,point1.y));
    border.set_position(1, sf::Vector3f(point2.x,height.y,point2.y));
    border.set_position(2, sf::Vector3f(point2.x,height.x,point2.y));
    border.set_position(3, sf::Vector3f(point1.x,height.x,point1.y));
    border.set_position(4, sf::Vector3f(point1.x,height.y,point1.y));
    panel.emplace_back(border);
   }
  else if(type == 2)
   {
    Object3d border(sf::LineStrip, 4);
    border.set_position(0, sf::Vector3f(point2.x,height.y,point2.y));
    border.set_position(1, sf::Vector3f(point1.x,height.y,point1.y));
    border.set_position(2, sf::Vector3f(point1.x,height.x,point1.y));
    border.set_position(3, sf::Vector3f(point2.x,height.x,point2.y));
    panel.emplace_back(border);
   }
  else if(type == 3)
   {
    Object3d border(sf::LineStrip, 4);
    border.set_position(0, sf::Vector3f(point1.x,height.y,point1.y));
    border.set_position(1, sf::Vector3f(point2.x,height.y,point2.y));
    border.set_position(2, sf::Vector3f(point2.x,height.x,point2.y));
    border.set_position(3, sf::Vector3f(point1.x,height.x,point1.y));
    panel.emplace_back(border);
   }
  else if(type == 4)
   {
    Object3d border(sf::Lines, 4);
    border.set_position(0, sf::Vector3f(point1.x,height.y,point1.y));
    border.set_position(1, sf::Vector3f(point2.x,height.y,point2.y));
    border.set_position(2, sf::Vector3f(point2.x,height.x,point2.y));
    border.set_position(3, sf::Vector3f(point1.x,height.x,point1.y));
    panel.emplace_back(border);
   }
  return panel;
 };

std::vector<std::vector<Object3d>> construct_door(sf::Vector2f point, bool orientation, float scale,sf::Color color = sf::Color::Blue)
 {
  std::vector<std::vector<Object3d>> polygons;

  std::vector<Object3d> polygon1; // wall
  Object3d pol1f(sf::TriangleStrip, 8, color);
   pol1f.set_position(0, sf::Vector3f((point.x*scale)+(scale*0.5), scale, point.y*scale));
   pol1f.set_position(1, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale));
   pol1f.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.5), 0, point.y*scale));
   pol1f.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale));
   pol1f.set_position(4, sf::Vector3f((point.x*scale)-(scale*0.5), 0, point.y*scale));
   pol1f.set_position(5, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale));
   pol1f.set_position(6, sf::Vector3f((point.x*scale)-(scale*0.5), scale, point.y*scale));
   pol1f.set_position(7, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale));
  polygon1.emplace_back(pol1f);
  Object3d pol1b(sf::LineStrip, 9);
  pol1b.set_position(0, sf::Vector3f((point.x*scale)+(scale*0.5), 0, point.y*scale));
  pol1b.set_position(1, sf::Vector3f((point.x*scale)+(scale*0.5), scale, point.y*scale));
  pol1b.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale));
  pol1b.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale));
  pol1b.set_position(4, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale));
  pol1b.set_position(5, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale));
  pol1b.set_position(6, sf::Vector3f((point.x*scale)-(scale*0.5), scale, point.y*scale));
  pol1b.set_position(7, sf::Vector3f((point.x*scale)-(scale*0.5), 0, point.y*scale));
  pol1b.set_position(8, sf::Vector3f((point.x*scale)+(scale*0.5), 0, point.y*scale));
  polygon1.emplace_back(pol1b);
  polygons.emplace_back(polygon1);

  std::vector<Object3d> polygon2; //front doorframe
  Object3d pol2f(sf::TriangleStrip, 8, color);
   pol2f.set_position(0, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale-0.05*scale));
   pol2f.set_position(1, sf::Vector3f((point.x*scale)+(scale*0.2), scale, point.y*scale-0.05*scale));
   pol2f.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale-0.05*scale));
   pol2f.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
   pol2f.set_position(4, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale-0.05*scale));
   pol2f.set_position(5, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
   pol2f.set_position(6, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale-0.05*scale));
   pol2f.set_position(7, sf::Vector3f((point.x*scale)-(scale*0.2), scale, point.y*scale-0.05*scale));
  polygon2.emplace_back(pol2f);
  Object3d pol2b(sf::LineStrip, 9);
  pol2b.set_position(0, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale-0.05*scale));
  pol2b.set_position(1, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale-0.05*scale));
  pol2b.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.2), scale, point.y*scale-0.05*scale));
  pol2b.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
  pol2b.set_position(4, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
  pol2b.set_position(5, sf::Vector3f((point.x*scale)-(scale*0.2), scale, point.y*scale-0.05*scale));
  pol2b.set_position(6, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale-0.05*scale));
  pol2b.set_position(7, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale-0.05*scale));
  pol2b.set_position(8, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale-0.05*scale));
  polygon2.emplace_back(pol2b);
  polygons.emplace_back(polygon2);

  std::vector<Object3d> polygon3; //back doorframe
  Object3d pol3f(sf::TriangleStrip, 8, color);
   pol3f.set_position(0, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale+0.05*scale));
   pol3f.set_position(1, sf::Vector3f((point.x*scale)+(scale*0.2), scale, point.y*scale+0.05*scale));
   pol3f.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale+0.05*scale));
   pol3f.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.2), 0.4*scale, point.y*scale+0.05*scale));
   pol3f.set_position(4, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale+0.05*scale));
   pol3f.set_position(5, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale+0.05*scale));
   pol3f.set_position(6, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale+0.05*scale));
   pol3f.set_position(7, sf::Vector3f((point.x*scale)-(scale*0.2), scale, point.y*scale+0.05*scale));
  polygon3.emplace_back(pol3f);
  Object3d pol3b(sf::LineStrip, 9);
  pol3b.set_position(0, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale+0.05*scale));
  pol3b.set_position(1, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale+0.05*scale));
  pol3b.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.2), scale, point.y*scale+0.05*scale));
  pol3b.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.2), 0.4*scale, point.y*scale+0.05*scale));
  pol3b.set_position(4, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale+0.05*scale));
  pol3b.set_position(5, sf::Vector3f((point.x*scale)-(scale*0.2), scale, point.y*scale+0.05*scale));
  pol3b.set_position(6, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale+0.05*scale));
  pol3b.set_position(7, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale+0.05*scale));
  pol3b.set_position(8, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale+0.05*scale));
  polygon3.emplace_back(pol3b);
  polygons.emplace_back(polygon3);

  std::vector<Object3d> polygon4; //right-front doorframe
  Object3d pol4f(sf::Quads, 4, color);
   pol4f.set_position(0, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale));
   pol4f.set_position(1, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale+0.05*scale));
   pol4f.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale+0.05*scale));
   pol4f.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale));
  polygon4.emplace_back(pol4f);
  Object3d pol4b(sf::LineStrip, 5);
  pol4b.set_position(0, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale));
  pol4b.set_position(1, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale+0.05*scale));
  pol4b.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale+0.05*scale));
  pol4b.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale));
  pol4b.set_position(4, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale));
  polygon4.emplace_back(pol4b);
  polygons.emplace_back(polygon4);

  std::vector<Object3d> polygon5; //right-back doorframe
  Object3d pol5f(sf::Quads, 4, color);
   pol5f.set_position(0, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale));
   pol5f.set_position(1, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale-0.05*scale));
   pol5f.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale-0.05*scale));
   pol5f.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale));
  polygon5.emplace_back(pol5f);
  Object3d pol5b(sf::LineStrip, 5);
  pol5b.set_position(0, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale));
  pol5b.set_position(1, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale-0.05*scale));
  pol5b.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale-0.05*scale));
  pol5b.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.3), 0.3*scale, point.y*scale));
  pol5b.set_position(4, sf::Vector3f((point.x*scale)+(scale*0.3), scale, point.y*scale));
  polygon5.emplace_back(pol5b);
  polygons.emplace_back(polygon5);

  std::vector<Object3d> polygon6; //left-front doorframe
  Object3d pol6f(sf::Quads, 4, color);
   pol6f.set_position(0, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale));
   pol6f.set_position(1, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale+0.05*scale));
   pol6f.set_position(2, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale+0.05*scale));
   pol6f.set_position(3, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale));
  polygon6.emplace_back(pol6f);
  Object3d pol6b(sf::LineStrip, 5);
  pol6b.set_position(0, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale));
  pol6b.set_position(1, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale+0.05*scale));
  pol6b.set_position(2, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale+0.05*scale));
  pol6b.set_position(3, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale));
  pol6b.set_position(4, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale));
  polygon6.emplace_back(pol6b);
  polygons.emplace_back(polygon6);

  std::vector<Object3d> polygon7; //left-back doorframe
  Object3d pol7f(sf::Quads, 4, color);
   pol7f.set_position(0, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale));
   pol7f.set_position(1, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale-0.05*scale));
   pol7f.set_position(2, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale-0.05*scale));
   pol7f.set_position(3, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale));
  polygon7.emplace_back(pol7f);
  Object3d pol7b(sf::LineStrip, 5);
  pol7b.set_position(0, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale));
  pol7b.set_position(1, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale-0.05*scale));
  pol7b.set_position(2, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale-0.05*scale));
  pol7b.set_position(3, sf::Vector3f((point.x*scale)-(scale*0.3), 0.3*scale, point.y*scale));
  pol7b.set_position(4, sf::Vector3f((point.x*scale)-(scale*0.3), scale, point.y*scale));
  polygon7.emplace_back(pol7b);
  polygons.emplace_back(polygon7);

  std::vector<Object3d> polygon8; //right doorframe inner
  Object3d pol8f(sf::Quads, 4, color);
   pol8f.set_position(0, sf::Vector3f((point.x*scale)+(scale*0.2), scale, point.y*scale-0.05*scale));
   pol8f.set_position(1, sf::Vector3f((point.x*scale)+(scale*0.2), scale, point.y*scale+0.05*scale));
   pol8f.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.2), 0.4*scale, point.y*scale+0.05*scale));
   pol8f.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
  polygon8.emplace_back(pol8f);
  Object3d pol8b(sf::LineStrip, 5);
  pol8b.set_position(0, sf::Vector3f((point.x*scale)+(scale*0.2), scale, point.y*scale-0.05*scale));
  pol8b.set_position(1, sf::Vector3f((point.x*scale)+(scale*0.2), scale, point.y*scale+0.05*scale));
  pol8b.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.2), 0.4*scale, point.y*scale+0.05*scale));
  pol8b.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
  pol8b.set_position(4, sf::Vector3f((point.x*scale)+(scale*0.2), scale, point.y*scale-0.05*scale));
  polygon4.emplace_back(pol8b);
  polygons.emplace_back(polygon8);

  std::vector<Object3d> polygon9; //left doorframe inner
  Object3d pol9f(sf::Quads, 4, color);
   pol9f.set_position(0, sf::Vector3f((point.x*scale)-(scale*0.2), scale, point.y*scale+0.05*scale));
   pol9f.set_position(1, sf::Vector3f((point.x*scale)-(scale*0.2), scale, point.y*scale-0.05*scale));
   pol9f.set_position(2, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
   pol9f.set_position(3, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale+0.05*scale));
  polygon9.emplace_back(pol9f);
  Object3d pol9b(sf::LineStrip, 5);
  pol9b.set_position(0, sf::Vector3f((point.x*scale)-(scale*0.2), scale, point.y*scale+0.05*scale));
  pol9b.set_position(1, sf::Vector3f((point.x*scale)-(scale*0.2), scale, point.y*scale-0.05*scale));
  pol9b.set_position(2, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
  pol9b.set_position(3, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
  pol9b.set_position(4, sf::Vector3f((point.x*scale)-(scale*0.2), scale, point.y*scale-0.05*scale));
  polygon9.emplace_back(pol9b);
  polygons.emplace_back(polygon9);

  std::vector<Object3d> polygon10; //top doorframe inner
  Object3d pol10f(sf::Quads, 4, color);
   pol10f.set_position(0, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale+0.05*scale));
   pol10f.set_position(1, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
   pol10f.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
   pol10f.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.2), 0.4*scale, point.y*scale+0.05*scale));
  polygon10.emplace_back(pol10f);
  Object3d pol10b(sf::LineStrip, 5);
  pol10b.set_position(0, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale+0.05*scale));
  pol10b.set_position(1, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
  pol10b.set_position(2, sf::Vector3f((point.x*scale)+(scale*0.2), 0.4*scale, point.y*scale-0.05*scale));
  pol10b.set_position(3, sf::Vector3f((point.x*scale)+(scale*0.2), 0.4*scale, point.y*scale+0.05*scale));
  pol10b.set_position(4, sf::Vector3f((point.x*scale)-(scale*0.2), 0.4*scale, point.y*scale+0.05*scale));
  polygon10.emplace_back(pol10b);
  polygons.emplace_back(polygon10);

  return polygons;
 }

class Map
 {
  public:
    Map(int X_, int Y_, std::vector<int> map_)
     {
      X = X_;
      Y = Y_;
      cX= (X*2)-1;
      cY= (Y*2)-1;

      if(map_.size() == X*Y)
       {
        std::cout<<"Map loaded susessfuly"<<std::endl;
        map = map_;
       }
      else
       {
        std::cout<<"MAP LOADING ERROR: IMPROPER MAP DIMENSIONS"<<std::endl;
       }

      for(int i=0;i<Y;++i) // stretches the original matrix to accomaodate more data later 1's a wall and 0's air
       {
        for(int j=0;j<X;++j) // emplaces 0 after every non-last member of the row
         {
          cv_map.emplace_back(map[(i*X)+j]);
          if(j != X-1)
           {cv_map.emplace_back(0);};
         }
        if(i != Y-1) // emplaces a row of 0's after every row that isnt the last
        {
         for(int j=0;j<cX;++j)
          {
           cv_map.emplace_back(0);
          }
        }
       }

//      std::cout<<cv_map.size()<<std::endl;

      for(int i=0;i<cY;++i) // puts 2's between blocks of air and wall, symbolising that a wall needs to be there
       {
        if(i%2 == 1)
         {
          for(int j=0;j<cX;j+=2)
           {
            if(((cv_map[i*cX+j-cX]==0||cv_map[i*cX+j-cX]==4) && cv_map[i*cX+j+cX] == 1)||((cv_map[i*cX+j+cX]==0||cv_map[i*cX+j+cX]==4) && cv_map[i*cX+j-cX] == 1))
             {
              cv_map[i*cX+j] = 2;
             }
            if((cv_map[i*cX+j-cX]==0 && cv_map[i*cX+j+cX] == 5)||(cv_map[i*cX+j+cX]==0 && cv_map[i*cX+j-cX] == 5))
            {
             cv_map[i*cX+j] = 6;
            }
           }
         }
        else if(i%2 == 0)
         {
          for(int j=1;j<cX;j+=2)
           {
            if((cv_map[i*cX+j-1] ==0 && cv_map[i*cX+j+1] == 1)||(cv_map[i*cX+j+1] == 0 && cv_map[i*cX+j-1] == 1))
             {
              cv_map[i*cX+j] = 2;
             }
           }
         }
       }

      for(int i=1;i<cY;i+=2) //places 3's between adjecent wall pannels, signifying a need for a seemless stransition
       {
        for(int j=1;j<cX;j+=2)
         {
          if((cv_map[i*cX+j-cX] == 2 && cv_map[i*cX+j+cX] == 2)||(cv_map[i*cX+j+1] == 2 && cv_map[i*cX+j-1] == 2))
           {
            cv_map[i*cX+j] = 3;
           }
         }
       }

      for(int i=0;i<cY;++i) // just a printer function
       {
        for(int j=0;j<cX;++j)
         {
          std::cout<<cv_map[i*cY+j]<<" ";
         }
        std::cout<<std::endl;
       }
     };

    sf::Vector3f get_spawnpoint()
     {
      return spawnpoint;
     }

    std::vector<sf::RectangleShape> render2d(sf::Vector3f offset, float scale, bool mode=0) //0 outputs walls, 1 outputs doors
     {
      std::vector<sf::RectangleShape> tiles;
      for(int i=0;i<Y;++i)
       {
        for(int j=0;j<X;++j)
         {
          if(map[i*X+j]==1&&mode==0)
           {
            sf::RectangleShape tile;
            tile.setSize(sf::Vector2f(scale,scale));
            tile.setPosition(j*scale,i*scale);
            tile.setFillColor(sf::Color::Red);
            tiles.emplace_back(tile);
           }
          else if(map[i*X+j]==5&&mode==1)
           {
            sf::RectangleShape door;
            door.setSize(sf::Vector2f(scale,scale));
            door.setPosition(j*scale,i*scale);
            door.setFillColor(sf::Color::Yellow);
            tiles.emplace_back(door);
           }
         }
       }
      return tiles;
     }

    std::vector<std::vector<Object3d>> render(sf::Vector3f offset, float scale) //scale will be the side of the square panels the maze ios built out of
     {
      std::vector<std::vector<Object3d>> Map;

      for(int i=0;i<cY;i++)
       {
        for(int j=0;j<cX;j++)
         {
          if(cv_map[i*cX+j] == 2)
           {
            if(i%2 == 1) //horizontal walls
             {
              if(cv_map[i*cX+j-1] == 3 && cv_map[i*cX+j+1] == 3)
               {
                //spawn in type 4 horizontally
                Map.emplace_back(construct_panel(sf::Vector2f((j*scale/2)-(scale/2), -i*scale/2), sf::Vector2f((j*scale/2)+(scale/2), -i*scale/2), sf::Vector2f(offset.y, scale), 4));
               }
              else if(cv_map[i*cX+j-1] == 3)
               {
                //spawn in type 3 horizontally
                Map.emplace_back(construct_panel(sf::Vector2f((j*scale/2)-(scale/2), -i*scale/2), sf::Vector2f((j*scale/2)+(scale/2), -i*scale/2), sf::Vector2f(offset.y, scale), 3));
               }
              else if(cv_map[i*cX+j+1] == 3)
               {
                //spawn in type 2 horizontally
                Map.emplace_back(construct_panel(sf::Vector2f((j*scale/2)-(scale/2), -i*scale/2), sf::Vector2f((j*scale/2)+(scale/2), -i*scale/2), sf::Vector2f(offset.y, scale), 2));
               }
              else
               {
                //spawn in type 1 horizontally
                Map.emplace_back(construct_panel(sf::Vector2f((j*scale/2)-(scale/2), -i*scale/2), sf::Vector2f((j*scale/2)+(scale/2), -i*scale/2), sf::Vector2f(offset.y, scale), 1));
               }
             }
            else if(i%2 == 0) //vertical walls
             {
              if(cv_map[i*cX+j-cX] == 3 && cv_map[i*cX+j+cX] == 3)
               {
                //spawn in type 4 vertically
                Map.emplace_back(construct_panel(sf::Vector2f(j*scale/2, -(i*scale/2)+(scale/2)), sf::Vector2f(j*scale/2, -(i*scale/2)-(scale/2)), sf::Vector2f(offset.y, scale), 4));
               }
              else if(cv_map[i*cX+j-cX] == 3)
               {
                //spawn in type 3 vertically
                Map.emplace_back(construct_panel(sf::Vector2f(j*scale/2, -(i*scale/2)+(scale/2)), sf::Vector2f(j*scale/2, -(i*scale/2)-(scale/2)), sf::Vector2f(offset.y, scale), 3));
               }
              else if(cv_map[i*cX+j+cX] == 3)
               {
                //spawn in type 2 vertically
                Map.emplace_back(construct_panel(sf::Vector2f(j*scale/2, -(i*scale/2)+(scale/2)), sf::Vector2f(j*scale/2, -(i*scale/2)-(scale/2)), sf::Vector2f(offset.y, scale), 2));
               }
              else
               {
                //spawn in type 1 vertically
                Map.emplace_back(construct_panel(sf::Vector2f(j*scale/2, -(i*scale/2)+(scale/2)), sf::Vector2f(j*scale/2, -(i*scale/2)-(scale/2)), sf::Vector2f(offset.y, scale), 1));
               }
             }
           }
          if(cv_map[i*cX+j] == 4)
           {
            spawnpoint = sf::Vector3f((j*scale/2), offset.y+(3*scale/4), -(i*scale/2));
           }
          if(cv_map[i*cX+j] == 6)
           {
            auto placeholder = construct_door(sf::Vector2f(j/2,(-i/2)-0.5),1,scale);
            for(auto piece : placeholder)
             {
              Map.emplace_back(piece);
             }
           }
         }
       }
      return Map;
     };


  private:
   int X,Y,cX,cY;
   std::vector<int> map;
   std::vector<int> cv_map;
   sf::Vector3f spawnpoint;
 };



int Partition(std::vector<std::vector<Object3d>> &v, int start, int end, Camera cam)
 {
  int pivot = end;
  int j = start;
  for(int i=start;i<end;++i)
   {
    if(distance_3d(v[i][0].get_com(),cam.get_pos())>distance_3d(v[pivot][0].get_com(),cam.get_pos()))
     {
      swap(v[i],v[j]);
      ++j;
     }
    }
  swap(v[j],v[pivot]);
  return j;
 }

void Oclusion(std::vector<std::vector<Object3d>> &v, int start, int end, Camera cam) // A quicksort algorythm i modified to work as an oclusion mechanism
{
 if(start<end)
  {
   int p = Partition(v,start,end,cam);
   Oclusion(v,start,p-1, cam);
   Oclusion(v,p+1,end, cam);
  }
}

int main()
{
    bool b_flag = false,c_flag = true;
    sf::RenderWindow window( sf::VideoMode( 1200, 800 ), "Mazer" );
    window.setFramerateLimit(60);
    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition(sf::Vector2i(window.getSize().x/2, window.getSize().y/2), window);

    sf::Clock clock;
    sf::Clock walk_clock;
    int n=255,m=0,o=0;

    std::vector<int> map1 {1,1,1,1,1,1,1,1,1,1,
                           1,0,1,0,0,4,0,0,0,1,
                           1,0,1,1,1,0,0,1,0,1,
                           1,0,0,0,1,0,1,1,0,1,
                           1,0,1,0,1,0,1,0,0,1,
                           1,0,1,0,0,0,1,1,1,1,
                           1,1,1,1,1,0,0,0,0,1,
                           1,0,0,0,1,1,0,1,0,1,
                           1,0,1,0,0,0,0,1,0,1,
                           1,5,1,1,1,1,1,1,1,1};

    std::vector<int> map2 {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
                           1,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,1,1,0,1,
                           1,1,1,0,1,0,0,0,1,0,1,1,0,0,0,1,1,0,0,1,
                           1,0,0,0,1,1,0,0,0,0,1,0,0,1,0,0,1,0,1,1,
                           1,0,1,0,0,0,0,1,0,1,1,0,1,1,1,0,0,0,0,1,
                           1,0,1,0,1,1,1,0,0,0,0,0,0,0,1,0,1,1,0,1,
                           1,1,0,0,0,1,0,0,0,1,0,1,1,0,0,0,1,0,0,1,
                           1,0,0,1,0,1,0,1,0,1,1,1,0,0,1,0,1,0,1,1,
                           1,0,1,1,0,0,0,0,0,0,0,0,0,1,1,0,1,0,0,1,
                           1,0,1,1,0,1,1,1,0,1,1,1,0,0,0,0,1,1,0,1,
                           1,0,0,0,0,0,0,1,0,0,0,1,1,1,1,0,0,0,0,1,
                           1,1,1,0,1,1,0,1,0,1,0,0,0,0,1,1,0,1,1,1,
                           1,0,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,1,
                           1,0,1,1,1,1,0,1,0,1,0,1,0,0,1,0,1,1,1,1,
                           1,0,0,0,1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1,
                           1,1,1,0,1,0,0,1,0,1,1,1,1,0,0,0,1,1,0,1,
                           1,0,0,0,0,0,4,0,0,0,0,1,0,0,1,1,1,0,0,1,
                           1,0,1,0,1,1,0,1,0,1,0,0,0,1,1,0,1,0,1,1,
                           1,0,1,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,
                           1,5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    Map Map1(20,20,map2);

    sf::RectangleShape start_screen(sf::Vector2f(window.getSize().x,window.getSize().y));
    sf::RectangleShape end_screen(sf::Vector2f(window.getSize().x,window.getSize().y));
    end_screen.setFillColor(sf::Color(255,255,255,0));
    start_screen.setFillColor(sf::Color::Black);

    sf::Font font;
    font.loadFromFile("RobotoStatic-Regular.ttf");

    sf::Text text("Press any button to start",font);
    text.setCharacterSize(40);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color::White);
    text.setPosition(350,700);

    sf::Text text1("You have escaped the maze",font);
    text1.setCharacterSize(40);
    text1.setStyle(sf::Text::Bold);
    text1.setFillColor(sf::Color(0,0,0,0));
    text1.setPosition(340,280);


//    Object3d coobe(sf::LineStrip,16);
//    std::vector<sf::Vector3f> points {sf::Vector3f(200, 200, 200), sf::Vector3f(400, 200, 200), sf::Vector3f(400, 400, 200), sf::Vector3f(200, 400, 200),
//                                      sf::Vector3f(200, 200, 400), sf::Vector3f(400, 200, 400), sf::Vector3f(400, 400, 400), sf::Vector3f(200, 400, 400)};
//    coobe.cube(points);

    sf::Vector2f origin (400,300);

    std::vector<std::vector<Object3d>> panels;
    std::vector<std::vector<Object3d>> doors;
    std::vector<sf::RectangleShape> tiles;
    std::vector<sf::RectangleShape> door_tiles;

    panels = Map1.render(sf::Vector3f(0,0,0),100);
    tiles = Map1.render2d(sf::Vector3f(0,0,0),100);
    door_tiles = Map1.render2d(sf::Vector3f(0,0,0),100,1);



//    coobe.rotate(sf::Vector3f(300,300,300),M_PI/4,1,1);
//    coobe.rotate(sf::Vector3f(300,300,300),M_PI/4,1,2);

    Camera Cam01(Map1.get_spawnpoint(),0, 30);

//    sf::VertexArray graph(sf::LineStrip, 3);
//    graph[0].position = sf::Vector2f(100,100);
//    graph[1].position = sf::Vector2f(100,500);
//    graph[2].position = sf::Vector2f(700,500);
//    Object2d test1(sf::Vector2f(200,200), 0, 50);
//    sf::CircleShape dot(5);
//    sf::VertexArray ray(sf::Lines, 2);
//    ray[0].color = sf::Color::Green;
//    ray[1].color = sf::Color::Green;


    while ( window.isOpen( ) )
    {
        sf::Time elapsed = clock.restart();
        sf::Event event;

        Cam01.move(elapsed, window, walk_clock.getElapsedTime(), tiles, door_tiles);
        Oclusion(panels,0,panels.size()-1, Cam01);

//        std::cout<<Cam01.get_state()<<std::endl;

        while( window.pollEvent( event ) )
        {
            if(event.type==sf::Event::KeyPressed)
             {
              b_flag = true;
              if(event.key.code==sf::Keyboard::W)
               {
                Cam01.set_state(1);
                walk_clock.restart();
               }
              if(event.key.code==sf::Keyboard::Escape)
               {
                window.close();
               }
             }
            if(event.type==sf::Event::KeyReleased)
             {
              if(event.key.code==sf::Keyboard::W)
               {
                Cam01.set_state(2);
               }
             }
            switch (event.type)
             {
            case sf::Event::Closed:
                window.close();
                break;
             }
        }

        if(b_flag==true&&c_flag==true)
         {
          start_screen.setFillColor(sf::Color(0,0,0,n));
          text.setFillColor(sf::Color(255,255,255,n));
          n-=2;
         }
        if(n<0)
         {
          c_flag=false;
         }

//        coobe.rotate(sf::Vector3f(300,300,300),M_PI/4,elapsed.asSeconds(),3);

        window.clear( );


//        window.draw(coobe.render());
//        window.draw(coobe.p_render(cam01, window.getSize()));
//        window.draw(graph);
//        window.draw(dot);
//        window.draw(ray);
//        window.draw(Pole.p_render(cam01, window.getSize()));
//        window.draw(Panel0.p_render(cam01, window.getSize()));
//        window.draw(Panel.p_render(cam01, window.getSize()));
//        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){

//        }
//        window.draw(start_screen);
        for(auto panel : panels)
         {
         for(auto part : panel)
          {
           window.draw(part.p_render(Cam01, window.getSize()));
          }
         }

        for(auto polygon : doors)
         {
         for(auto part : polygon)
          {
           window.draw(part.p_render(Cam01, window.getSize()));
          }
         }
//        for(auto tile : tiles)
//         {
//          window.draw(tile);
//         }
//        window.draw(Cam01.get_mini());
        if(c_flag){
            window.draw(start_screen);
            window.draw(text);
        }
        if(Cam01.get_exited()==true)
         {
          window.draw(end_screen);
          end_screen.setFillColor(sf::Color(255,255,255,m));
          if(m<254){m+=2;}
          else
           {
            window.draw(text1);
            text1.setFillColor(sf::Color(0,0,0,o));
            if(o<254){o+=2;}
           }
         }

        window.display( );
    }
}
