#include "SFML/Graphics.hpp"
#include <SFML/Window.hpp>
#include <iostream>
#include <vector>
#include <cmath>

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

    void move(const sf::Time &elapsed)
     {

//      std::cout<<fov<<std::endl;

      if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
       {
        position.z += 100*elapsed.asSeconds()*cos(turn*M_PI/180);
        position.x -= 100*elapsed.asSeconds()*sin(turn*M_PI/180);
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
       {
        position.z -= 100*elapsed.asSeconds()*cos(turn*M_PI/180);
        position.x += 100*elapsed.asSeconds()*sin(turn*M_PI/180);
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
       {
        position.z += 100*elapsed.asSeconds()*sin(turn*M_PI/180);
        position.x += 100*elapsed.asSeconds()*cos(turn*M_PI/180);
       }
      if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
       {
        position.z -= 100*elapsed.asSeconds()*sin(turn*M_PI/180);
        position.x -= 100*elapsed.asSeconds()*cos(turn*M_PI/180);
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



      if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
       {
        turn += 50*elapsed.asSeconds();
       }
      if(sf::Mouse::isButtonPressed(sf::Mouse::Right))
       {
        turn -= 50*elapsed.asSeconds();
       }
     }

  private:
   sf::Vector3f position;
   float turn;
   float fov;
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

class Map
 {
  public:
    Map(int X_, int Y_, std::vector<int> map_)
     {
      X = X_;
      Y = Y_;
      cX= (X*2)-1;
      cY= (Y*2)-1;
//      std::vector<int> cv_map;

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
        for(int j=0;j<X;++j)
         {
          cv_map.emplace_back(map[(i*X)+j]);
          if(j != X-1)
           {cv_map.emplace_back(0);};
         }
        if(i != Y-1)
        {
         for(int j=0;j<cX;++j)
          {
           cv_map.emplace_back(0);
          }
        }
       }

      std::cout<<cv_map.size()<<std::endl;

      for(int i=0;i<cY;++i) // puts 2's between blocks of air and wall, symbolising that a wall needs to be there
       {
        if(i%2 == 1)
         {
          for(int j=0;j<cX;j+=2)
           {
            if((cv_map[i*cX+j-cX] ==0 && cv_map[i*cX+j+cX] == 1)||(cv_map[i*cX+j+cX] ==0 && cv_map[i*cX+j-cX] == 1))
             {
              cv_map[i*cX+j] = 2;
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

      for(int i=0;i<cY;++i)
       {
        for(int j=0;j<cX;++j)
         {
          std::cout<<cv_map[i*cY+j]<<" ";
         }
        std::cout<<std::endl;
       }
     };

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
                Map.emplace_back(construct_panel(sf::Vector2f((j*scale/2)-(scale/2), i*scale/2), sf::Vector2f((j*scale/2)+(scale/2), i*scale/2), sf::Vector2f(0, scale), 4));
               }
              else if(cv_map[i*cX+j-1] == 3)
               {
                //spawn in type 3 horizontally
                Map.emplace_back(construct_panel(sf::Vector2f((j*scale/2)-(scale/2), i*scale/2), sf::Vector2f((j*scale/2)+(scale/2), i*scale/2), sf::Vector2f(0, scale), 3));
               }
              else if(cv_map[i*cX+j+1] == 3)
               {
                //spawn in type 2 horizontally
                Map.emplace_back(construct_panel(sf::Vector2f((j*scale/2)-(scale/2), i*scale/2), sf::Vector2f((j*scale/2)+(scale/2), i*scale/2), sf::Vector2f(0, scale), 2));
               }
              else
               {
                //spawn in type 1 horizontally
                Map.emplace_back(construct_panel(sf::Vector2f((j*scale/2)-(scale/2), i*scale/2), sf::Vector2f((j*scale/2)+(scale/2), i*scale/2), sf::Vector2f(0, scale), 1));
               }
             }
            else if(i%2 == 0) //vertical walls
             {
              if(cv_map[i*cX+j-cX] == 3 && cv_map[i*cX+j+cX] == 3)
               {
                //spawn in type 4 vertically
                Map.emplace_back(construct_panel(sf::Vector2f(j*scale/2, (i*scale/2)-(scale/2)), sf::Vector2f(j*scale/2, (i*scale/2)+(scale/2)), sf::Vector2f(0, scale), 4));
               }
              else if(cv_map[i*cX+j-cX] == 3)
               {
                //spawn in type 3 vertically
                Map.emplace_back(construct_panel(sf::Vector2f(j*scale/2, (i*scale/2)-(scale/2)), sf::Vector2f(j*scale/2, (i*scale/2)+(scale/2)), sf::Vector2f(0, scale), 3));
               }
              else if(cv_map[i*cX+j+cX] == 3)
               {
                //spawn in type 2 vertically
                Map.emplace_back(construct_panel(sf::Vector2f(j*scale/2, (i*scale/2)-(scale/2)), sf::Vector2f(j*scale/2, (i*scale/2)+(scale/2)), sf::Vector2f(0, scale), 2));
               }
              else
               {
                //spawn in type 1 vertically
                Map.emplace_back(construct_panel(sf::Vector2f(j*scale/2, (i*scale/2)-(scale/2)), sf::Vector2f(j*scale/2, (i*scale/2)+(scale/2)), sf::Vector2f(0, scale), 1));
               }
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

 };



int Partition(std::vector<std::vector<Object3d>> &v, int start, int end, Camera cam){

    int pivot = end;
    int j = start;
    for(int i=start;i<end;++i){
        if(distance_3d(v[i][0].get_com(),cam.get_pos())>distance_3d(v[pivot][0].get_com(),cam.get_pos())){
            swap(v[i],v[j]);
            ++j;
        }
    }
    swap(v[j],v[pivot]);
    return j;
}
void Oclusion(std::vector<std::vector<Object3d>> &v, int start, int end, Camera cam) // A quicksort algorythm i yoinked from the internet and modify to work as an oclusion mechanism
{
 if(start<end)
  {
   int p = Partition(v,start,end,cam);
   Oclusion(v,start,p-1, cam);
   Oclusion(v,p+1,end, cam);
  }
}

//void PrintVector(std::vector<std::vector<Object3d>> v){
//    for(int i=0;i<v.size();++i)
//        std::cout<<v[i][0].get_com()<<" ";
//    cout<<"\n\n";
//}

//void Oclusion(std::vector<std::vector<Object3d>> &panels, Camera cam01)
// {
//  Quicksort(panels,0,panels.size()-1, cam01);
// };

int main()
{
    sf::RenderWindow window( sf::VideoMode( 800, 600 ), "SFML WORK!" );
    window.setFramerateLimit(60);
    sf::Clock clock;

    std::vector<int> map1 {1,1,1,1,1,1,1,1,1,1,
                           1,0,1,0,0,0,0,0,0,1,
                           1,0,1,1,1,0,0,1,0,1,
                           1,0,0,0,1,0,1,1,0,1,
                           1,0,1,0,1,0,1,0,0,1,
                           1,0,1,0,0,0,1,1,1,1,
                           1,1,1,1,1,0,0,0,0,1,
                           1,0,0,0,1,1,0,1,0,1,
                           1,0,1,0,0,0,0,1,0,1,
                           1,1,1,1,1,1,1,1,1,1};
    Map Map1(10,10,map1);


    std::vector<sf::RectangleShape> rectangles;
//    for(int i=0; i<map1.size(); i++){       //filling rectangles vector
//        if(map1[i]==1){
//            sf::RectangleShape shape;
//            rectangles.push_back(shape);
//        }

//    }

    for(int i=0;i<10;++i)
     {
      for(int j=0;j<10;++j)
       {
          sf::RectangleShape shape;
          shape.setPosition(j*50,i*50);
          rectangles.emplace_back(shape);
    }}



    Object3d coobe(sf::LineStrip,16);
    std::vector<sf::Vector3f> points {sf::Vector3f(200, 200, 200), sf::Vector3f(400, 200, 200), sf::Vector3f(400, 400, 200), sf::Vector3f(200, 400, 200),
                                      sf::Vector3f(200, 200, 400), sf::Vector3f(400, 200, 400), sf::Vector3f(400, 400, 400), sf::Vector3f(200, 400, 400)};
    coobe.cube(points);

    sf::Vector2f origin (400,300);

    std::vector<std::vector<Object3d>> panels;
//    std::vector<Object3d> panel1 = construct_panel(sf::Vector2f(100,100),sf::Vector2f(100,200),sf::Vector2f(300,200),1, sf::Color::Red);
//    panels.emplace_back(panel1);
//    std::vector<Object3d> panel2 = construct_panel(sf::Vector2f(100,200),sf::Vector2f(200,200),sf::Vector2f(300,200),2, sf::Color::Blue);
//    panels.emplace_back(panel2);
//    std::vector<Object3d> panel3 = construct_panel(sf::Vector2f(200,200),sf::Vector2f(200,100),sf::Vector2f(300,200),3, sf::Color::Green);
//    panels.emplace_back(panel3);
//    std::vector<Object3d> panel4 = construct_panel(sf::Vector2f(200,100),sf::Vector2f(100,100),sf::Vector2f(300,200),4, sf::Color::Yellow);
//    panels.emplace_back(panel4);
    panels = Map1.render(sf::Vector3f(0,0,0),100);

//    coobe.rotate(sf::Vector3f(300,300,300),M_PI/4,1,1);
//    coobe.rotate(sf::Vector3f(300,300,300),M_PI/4,1,2);

    Camera cam01(sf::Vector3f(300,300,-500),0, 30);

//    sf::VertexArray graph(sf::LineStrip, 3);
//    graph[0].position = sf::Vector2f(100,100);
//    graph[1].position = sf::Vector2f(100,500);
//    graph[2].position = sf::Vector2f(700,500);
//    Object2d test1(sf::Vector2f(200,200), 0, 50);
//    sf::CircleShape dot(5);
//    sf::VertexArray ray(sf::Lines, 2);
//    ray[0].color = sf::Color::Green;
//    ray[1].color = sf::Color::Green;


    sf::RectangleShape player;


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
        player.setPosition(cam01.get_pos().x,cam01.get_pos().y); //player tracking the camera
        Oclusion(panels,0,panels.size()-1, cam01);
//        std::cout<<"Red: "<<distance_3d(panel1[0].get_com(), cam01.get_pos())<<" Blue: "<<distance_3d(panel2[0].get_com(), cam01.get_pos())
//                 <<" Green: "<<distance_3d(panel3[0].get_com(), cam01.get_pos())<<" Yellow: "<< distance_3d(panel4[0].get_com(), cam01.get_pos())
//                 <<" Camera X: "<<cam01.get_pos().x<<" Y: "<<cam01.get_pos().y<<" Z: "<<cam01.get_pos().z<<std::endl;

//        std::cout<<"X: "<<cam01.get_pos().x<<" Y: "<<cam01.get_pos().y<<" Z: "<<cam01.get_pos().z<<" Turn: "<<cam01.get_tur()<<std::endl;

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
//        window.draw(coobe.p_render(cam01, window.getSize()));
//        window.draw(graph);
//        window.draw(dot);
//        window.draw(ray);
//        window.draw(Pole.p_render(cam01, window.getSize()));
//        window.draw(Panel0.p_render(cam01, window.getSize()));
//        window.draw(Panel.p_render(cam01, window.getSize()));
        for(auto panel : panels)
         {
         for(auto part : panel)
          {
           window.draw(part.p_render(cam01, window.getSize()));
          }
         }
        window.display( );
    }
}
