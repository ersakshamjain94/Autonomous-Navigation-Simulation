#include <SFML/Graphics.hpp>
#include<math.h>
#include<stdlib.h>
#define rotation 5.0
#define step 5.0
#include<iostream>
using namespace std;
int main()
{   double velocity=0.0;
    sf::RenderWindow window(sf::VideoMode(800,600),"Training",sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    sf::CircleShape car(10,3); //creates a triangle actually because we said circle is created with 3 lines
    car.setOrigin(car.getRadius(), car.getRadius()); //very much necessary else it ship will rotate about 0,0
    car.setPosition(sf::Vector2f(100,100)); //default position of car is on the top left
    car.setFillColor(sf::Color::Red); //set the color of car to be red
    car.setRotation(0); //default orientation

    while(window.isOpen())
    {    window.clear(sf::Color::Black); //sets background color
    window.draw(car); //draws but doesnt display
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
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                car.rotate(-rotation);
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                float x,y; //to make move in direction of pointing
                x = step*(float)sin((M_PI / 180.0f) * (360.0f-car.getRotation())); //for reverse gear
                 y = step*(float)cos((M_PI / 180.0f) * (360.0f-car.getRotation()));
                car.move(x,y);
                cout<<"moving new position is->"<<car.getPosition().x<<" "<<car.getPosition().y<<endl;

                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {   float x,y; //to make move in direction of pointing
                x = step*(float)sin((M_PI / 180.0f) * (car.getRotation()));
                 y = step*(float)cos((M_PI / 180.0f) * (car.getRotation()));    //for forward gear
                car.move(x,-y);
                cout<<"moving new position is->"<<car.getPosition().x<<" "<<car.getPosition().y<<endl;
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

