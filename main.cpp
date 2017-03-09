#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<math.h>
#include<stdlib.h>
#define rotation 5.0
#define step 5.0
#include<iostream>
#include<stdlib.h>
using namespace std;
//remember positions are defined by top left corner of rectangle covering any object always until we change it
//views are always modified by their centre


/*
class Car:public sf::Shape
{
    public:
    sf::CircleShape body; //our car
   // sf::CircleShape field; //field of view
    Car(float radius,float radius_car,sf::Vector2f C,sf::Vector2f V)
    {
        body.setFillColor(sf::Color::Red);
     //   field.setFillColor(sf::Color::Transparent);
        body.setRadius(radius_car);
       // field.setRadius(radius);
        body.setPointCount(4);
      //  body.setOrigin(radius_car,radius_car);
       // field.setOrigin(radius);
        body.setPosition(V);
       // field.setPosition(C);
        body.setPointCount(3);
        update();
    }
     virtual std::size_t getPointCount() const
    {

        return body.getPointCount();
    }

    virtual sf::Vector2f getPoint(unsigned int index) const
    {
        static const float pi = 3.141592654f;

        float angle = index * 2 * pi / getPointCount() - pi / 2;
        float x = cos(angle) * body.getRadius();
        float y = sin(angle) * body.getRadius();

        return sf::Vector2f(body.getRadius() + x, body.getRadius() + y);
    }
};*/
void createObstacles(sf::CircleShape obstacle [],sf::Vector2u windowSize,int pointCount,int radius,int n)
{

        int i;
        int r,g,b;
        int x,y;
        srand(time(0));

        for(i=0;i<n;i++)
        {
            x=rand()%windowSize.x;
            y=rand()%windowSize.y;
           // cout<<"\nObstacle created at->"<<x<<" "<<y<<endl;
            r=rand()%255;
            g=rand()%255;
            b=rand()%255;
            sf::Color color(r,g,b);
            (obstacle[i]).setFillColor(color);
            (obstacle[i]).setRadius(radius);
            (obstacle[i]).setPointCount(pointCount);
            obstacle[i].setPosition(x,y);
            obstacle[i].setOrigin(radius,radius);

        }

}
vector<sf::CircleShape> getProximity(sf::CircleShape obstacle[],int num_obstacles,sf::Vector2f fieldPosition,float radius)
{       vector<sf::CircleShape>result;
        float fieldx=fieldPosition.x;
        float fieldy=fieldPosition.y;

    int i;
    for(i=0;i<num_obstacles;i++)
    {   float obstaclex=obstacle[i].getPosition().x;
        float obstacley=obstacle[i].getPosition().y;
        double distance=(pow((obstaclex-fieldx),2)+pow((obstacley-fieldy),2));
        distance=sqrt(distance);
        cout<<"distance "<<distance<<endl;
        if(distance<=radius)
        result.push_back(obstacle[i]); //obstacle i is in field of view
    }
    return result;
}
void printProximity(sf::CircleShape obstacle [],int num_obstacles,sf::Vector2f fieldPosition,sf::Vector2f carPosition,float radius)
{   vector<sf::CircleShape>result=getProximity(obstacle,num_obstacles,fieldPosition,radius);
    vector<sf::CircleShape> ::iterator it;
    cout<<"\nobjects in proximity are->"<<endl;
    //forming the equation of line joining carPosition and fieldPosition(x2,y2)
    double m=(double)(fieldPosition.y-carPosition.y)/(double)(fieldPosition.x-carPosition.x); //slope of line
    // y-y1 = m*(x-x1) if satisfied , x,y lies on the line now refer to stackoverflow answer here
    //http://math.stackexchange.com/questions/1435779/calculate-if-a-point-lies-above-or-below-or-right-to-left-of-a-line

    for(it=result.begin();it!=result.end();it++)
    {
        sf::CircleShape temp=*it;
        double x=temp.getPosition().x;
        double y=temp.getPosition().y
        cout<<"X="<<x<<" "<<"y="<<y<<endl;

    }

}
void collisionDetected(sf::SoundBuffer buffer)
{

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();
}

using namespace std;
int main()
{   int i;
    sf::RenderWindow window(sf::VideoMode(1024,700),"Training",sf::Style::Close);
    //creating obstacles
    window.setVerticalSyncEnabled(true);
    sf::CircleShape car(20,3); //creates a triangle actually because we said circle is created with 3 lines
    car.setOrigin(car.getRadius(), car.getRadius()); //very much necessary else it ship will rotate about 0,0
    car.setPosition(sf::Vector2f(500,500)); //default position of car is on the top left we are setting position of centre of car
    car.setFillColor(sf::Color::Red); //set the color of car to be red
    car.setRotation(0); //default orientation
    sf::CircleShape obstacle[100];
    sf::Vector2u bounds=window.getSize();
    sf::CircleShape field(20); //taking radius of field of view as 10

   //// field.setOrigin(car.getPosition());
   float fieldOffset=car.getRadius()+3*field.getRadius();
   field.setPosition(0,-fieldOffset);
    //field.move(car.getPosition().x-field.getRadius(),car.getPosition().y-2*field.getRadius());
    field.setFillColor(sf::Color::Blue);
    field.setRotation(0);
    float fieldOriginOffset=car.getRadius()+field.getRadius();
    field.setOrigin(0,-fieldOriginOffset);
    int radius_obstacle,number_obstacle,sides_obstacle;
    cout<<"\nEnter the radius of obstacle->";
    //cin>>size_obstacle;
    radius_obstacle=10;
    cout<<"\nEnter the number of obstacles->";
    //cin>>number_obstacle;
    number_obstacle=10;
    cout<<"\nEnter the number of sides each obstacle have->";
    //cin>>sides_obstacle;
    sides_obstacle=5;
    createObstacles(obstacle,bounds,sides_obstacle,radius_obstacle,number_obstacle);
    for(i=0;i<number_obstacle;i++)
    {
        sf::Vector2f temp=obstacle[i].getPosition();
        cout<<"obstacle "<<i<<" at "<<"x="<<temp.x<<"y="<<temp.y<<endl;
    }
    sf::FloatRect obstacleRect[1000];
    for(i=0;i<number_obstacle;i++)
    {
        obstacleRect[i]=obstacle[i].getGlobalBounds(); //getting a rect for each obstacle
    }
    sf::FloatRect carRect=car.getGlobalBounds();//getting a bounding rectangle for car;
    sf::FloatRect fieldRect=field.getGlobalBounds(); //getting bouding rectangle for the field
   /* sf::CircleShape p1(1);
    p1.setPosition(car.getOrigin());
    cout<<"\np1="<<p1.getPosition().x<<" "<<p1.getPosition().y;
    p1.setFillColor(sf::Color::Magenta);
    sf::CircleShape p2(1);
    p2.setPosition(field.getOrigin());
    p2.setFillColor(sf::Color::White);
    cout<<"\np2="<<p2.getPosition().x<<" "<<p2.getPosition().y;
*/
    sf::SoundBuffer buffer;

    bool loaded=buffer.loadFromFile("sound.wav");
    if(loaded==false)
    {
        cout<<"\nEnter correct path to audio file";
        exit(0);
    }

while(window.isOpen())
{
    window.clear(sf::Color::Black); //sets background color
    window.draw(car); //draws but doesnt display
    sf::Transform fieldTransform=car.getTransform(); //copies transformation of car to the field

    window.draw(field,fieldTransform); //just shows field as if its moving, its position is fixed not moving it just appears to be moving
    //followint two lines are very important please dont change them
    sf::Vector2f fieldpositionwithoutoffset(0+field.getRadius(),0-field.getRadius());
    sf::Vector2f pos_circle=fieldTransform.transformPoint(fieldpositionwithoutoffset); //we are calculating position of field expicitely
   // pos_circle.y=pos_circle.y+fieldOffset;
    //sets the position of field for real
    //window.draw(field);
    // window.draw(p1);
    // window.draw(p2);
    for(i=0;i<number_obstacle;i++)
    {window.draw(obstacle[i]);
      sf::Vertex v(obstacle[i].getPosition()); //this segment of code is to draw a point at each position of the obstacle
        window.draw(&v, 1, sf::Points);
    }
    sf::Vertex carPosvertex(car.getPosition());
    sf::Vertex fieldPosvertex(pos_circle);
    window.draw(&carPosvertex,1,sf::Points);
    window.draw(&fieldPosvertex,1,sf::Points);
    window.display();

        sf::Event event;
        while(window.pollEvent(event))
        {

            if(event.type==sf::Event::Closed)
            window.close();
            if(event.type==sf::Event::KeyPressed)
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    car.rotate(rotation);
                    //field.rotate(rotation);
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {car.rotate(-rotation);
                //field.rotate(-rotation);
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {

                float x,y; //to make move in direction of pointing
                x = step*(float)sin((M_PI / 180.0f) * (360.0f-car.getRotation())); //for reverse gear
                 y = step*(float)cos((M_PI / 180.0f) * (360.0f-car.getRotation()));
                car.move(x,y);
                carRect=car.getGlobalBounds();
                cout<<"car position->"<<car.getPosition().x<<" "<<car.getPosition().y<<endl;
                cout<<"carRect->"<<carRect.top<<endl;
                cout<<"field position->"<<pos_circle.x<<" "<<pos_circle.y<<endl;
                //cout<<"field origin->"<<field.getOrigin().x<<" "<<field.getOrigin().y<<endl;
                printProximity(obstacle,number_obstacle,pos_circle,car.getPosition(),field.getRadius());
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {

                float x,y; //to make move in direction of pointing
                x = step*(float)sin((M_PI / 180.0f) * (car.getRotation()));
                 y = step*(float)cos((M_PI / 180.0f) * (car.getRotation()));    //for forward gear
                car.move(x,-y);
                carRect=car.getGlobalBounds();
                cout<<"car position->"<<car.getPosition().x<<" "<<car.getPosition().y<<endl;
                cout<<"carRect->"<<carRect.top<<endl;
                cout<<"field position->"<<pos_circle.x<<" "<<pos_circle.y<<endl;
               // cout<<"field origin->"<<field.getOrigin().x<<" "<<field.getOrigin().y<<endl;
                printProximity(obstacle,number_obstacle,pos_circle,car.getPosition(),field.getRadius()); //pos_circle is field position
                }

            }
            if(event.type==sf::Event::KeyReleased)
            {
                    if(event.key.code==sf::Keyboard::Right)
                    cout<<"New angle is set and it is->"<<car.getRotation()<<endl;
                    if(event.key.code==sf::Keyboard::Left)
                    cout<<"New angle is set and it is->"<<car.getRotation()<<endl;


            }

        }



    }
}

