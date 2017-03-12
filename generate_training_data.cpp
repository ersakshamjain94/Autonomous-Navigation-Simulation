#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<math.h>
#include<stdlib.h>
#define rotation 5.0
#define step 5.0
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#define placeholder 999
#define maxupdown 10 //used for scaling up and down moves
using namespace std;
//remember positions are defined by top left corner of rectangle covering any object always until we change it
//views are always modified by their centre

struct record {
    double left;
    double front;
    double right;
    double rotations;
    double up; //accelration deaccelration
};
class objects { //represents objects in field of view
   public:
    int side; //left right or straight
    double distance;
};
bool compareByDistance(const objects &a, const objects &b)
{
    return a.distance < b.distance;
}
void displayRecord(struct record * r)
{
    cout<<r->left<<" "<<r->front<<" "<<r->right<<" "<<r->up<<" "<<r->rotations<<endl;

}
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
        //cout<<"distance "<<distance<<endl; //uncomment to see distances of all objects
        if(distance<=radius)
        result.push_back(obstacle[i]); //obstacle i is in field of view
    }
    return result;
}
struct record * printProximity(sf::CircleShape obstacle [],int num_obstacles,sf::Vector2f fieldPosition,sf::Vector2f carPosition,float radius,struct record * r,sf::Event event,sf::CircleShape car)
{   FILE * fout=fopen("data.txt","a+");

    //http://math.stackexchange.com/questions/1435779/calculate-if-a-point-lies-above-or-below-or-right-to-left-of-a-line
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)||sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
     vector<sf::CircleShape>result=getProximity(obstacle,num_obstacles,fieldPosition,radius);
    vector<sf::CircleShape> ::iterator it;
    cout<<"\nobjects in proximity are->"<<endl;
    //forming the equation of line joining carPosition and fieldPosition(x2,y2)
    double x1,x2,y1,y2;
    x1=carPosition.x;
    y1=carPosition.y;
    x2=fieldPosition.x;
    y2=fieldPosition.y;
    if(result.empty()==true)
    {
       // r->front=1;
       // r->left=1;
        //r->right=1;
       // r->up=1;
        //r->rotations=0;
        return r;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        r->up=1;
    else
        r->up=0;
    int position;
    int finalposition=3;
    vector<objects> obstaclesInRange;
        double dist;
    for(it=result.begin();it!=result.end();it++) //scan all the objects in the field of view
    {   struct objects singleobstacle;
        sf::CircleShape temp=*it;
        double x=temp.getPosition().x;
        double y=temp.getPosition().y; //also called Yp
        cout<<"X="<<x<<" "<<"y="<<y<<endl;
        double value=(x2-x1)*(y-y1)-(y2-y1)*(x-x1);
        if(value==0)
        position=1; //front
        else if(value<0)
        position=0; //left of the line
        else
        position=2; //right
        singleobstacle.side=position;
        dist=pow((x-x1),2)+pow((y-y1),2);
        dist=sqrt(dist);
        dist=dist/(2*radius+car.getRadius());
        singleobstacle.distance=dist;
        obstaclesInRange.push_back(singleobstacle);

    }
    //std::sort(obstaclesInRange.begin(),obstaclesInRange.end(),compareByDistance);
        objects closestLeft,closestUp,closestRight; //we want to find these 3 objects in three directions
        closestLeft.distance=2; //default distance
        closestUp.distance=2;
        closestRight.distance=2;
        closestLeft.side=0; //setting direction to be left side
        closestUp.side=1;
        closestRight.side=2;
       vector<objects>::iterator objectsIterator;
        for(objectsIterator=obstaclesInRange.begin();objectsIterator!=obstaclesInRange.end();objectsIterator++)
        {
            if(((*objectsIterator).side)==0 ) //side = left
            {
                if(((*objectsIterator).distance)<(closestLeft.distance))
                closestLeft.distance=((*objectsIterator).distance);
            }
            if(((*objectsIterator).side)==1 ) //side = straight
            {
                if(((*objectsIterator).distance)<closestUp.distance)
                closestUp.distance=((*objectsIterator).distance);
            }
            if(((*objectsIterator).side)==2 ) //side = right
            {
                    if(((*objectsIterator).distance)<closestRight.distance)
                closestRight.distance=((*objectsIterator).distance);
            }

        }

        if(closestLeft.distance<=1)
        {
            r->left=closestLeft.distance;
        }
        if(closestRight.distance<=1) //checks if there is any object to the right
        {
            r->right=closestRight.distance;
        }
         if(closestUp.distance<=1)
        {
            r->front=closestUp.distance;
        }
        if(r->left>1)
        r->left=1;
        if(r->front>1)
        r->front=1;
        if(r->right>1)
        r->right=1;
         cout<<"\nClosest distances are->"<<closestLeft.distance<<" "<<closestUp.distance<<" "<<closestRight.distance;
    }

        return r;
}
void collisionDetected(sf::SoundBuffer buffer)
{

    sf::Sound sound;
    sound.setBuffer(buffer);
    sound.play();
}

using namespace std;
int main()
{   FILE * fout= fopen("data.txt","a+");
    int i;
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
    number_obstacle=100;
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
     struct record * r = (struct record *)malloc(sizeof(struct record));
    r->front=placeholder;
    r->left=placeholder;
    r->right=placeholder;
    r->up=placeholder;
    r->rotations=placeholder;
    float number_of_right_rotations=0;
    float number_of_left_rotations=0;
    float number_of_up=0;
    float number_of_down=0;
     int flag=0;
     int flagMove=0;
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
        //flag used to check whtehr we are rotating because of obstacle or just like that
        // 0 represents just like that , 1 represents obstacle is there

        while(window.pollEvent(event))
        {   if(r->front!=placeholder && r->left!= placeholder && r->right != placeholder && r->rotations != placeholder && r->up != placeholder)
                {   printf("writing to the file %lf %lf %lf %lf %lf\n",r->left,r->front,r->right,r->up,r->rotations);
                    fprintf(fout,"%lf %lf\n%lf\n",r->left,r->right,r->rotations);
                    r->front=placeholder;
                    r->left=placeholder;
                    r->right=placeholder;
                    r->up=placeholder;
                    r->rotations=placeholder;

                }
            if(event.type==sf::Event::Closed)
            {window.close();
             fclose(fout);
             }
            if(event.type==sf::Event::KeyPressed)
            {    vector<sf::CircleShape>result=getProximity(obstacle,number_obstacle,pos_circle,field.getRadius());
                     if(result.empty()!=true)
                     {flag=1;
                        flagMove=1;
                     cout<<"\nflag activated";
                     }
                displayRecord(r);
                r=printProximity(obstacle,number_obstacle,pos_circle,car.getPosition(),field.getRadius(),r,event,car);
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                {
                    if(flag==1)
                    number_of_right_rotations++;
                    cout<<"\nright rotating";

                    car.rotate(rotation);
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                {
                     if(flag==1)
                      number_of_left_rotations++;
                     cout<<"\nleft rotating";

                    car.rotate(-rotation);
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                if(flagMove==1)
                number_of_down++;
                float x,y; //to make move in direction of pointing
                x = step*(float)sin((M_PI / 180.0f) * (360.0f-car.getRotation())); //for reverse gear
                 y = step*(float)cos((M_PI / 180.0f) * (360.0f-car.getRotation()));
                car.move(x,y);
                carRect=car.getGlobalBounds();
                cout<<"car position->"<<car.getPosition().x<<" "<<car.getPosition().y<<endl;
                cout<<"carRect->"<<carRect.top<<endl;
                cout<<"field position->"<<pos_circle.x<<" "<<pos_circle.y<<endl;
                //cout<<"field origin->"<<field.getOrigin().x<<" "<<field.getOrigin().y<<endl;

               //
                //r->up=0; //down key pressed
                }
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                {
                if(flagMove==1)
                number_of_up++;
                float x,y; //to make move in direction of pointing
                x = step*(float)sin((M_PI / 180.0f) * (car.getRotation()));
                 y = step*(float)cos((M_PI / 180.0f) * (car.getRotation()));    //for forward gear
                car.move(x,-y);
                carRect=car.getGlobalBounds();
                cout<<"car position->"<<car.getPosition().x<<" "<<car.getPosition().y<<endl;
                cout<<"carRect->"<<carRect.top<<endl;
                cout<<"field position->"<<pos_circle.x<<" "<<pos_circle.y<<endl;
               // cout<<"field origin->"<<field.getOrigin().x<<" "<<field.getOrigin().y<<endl;

                //r=printProximity(obstacle,number_obstacle,pos_circle,car.getPosition(),field.getRadius(),r); //pos_circle is field position
                //r->up=1; //up key pressed

                }


            }

            if(event.type==sf::Event::KeyReleased)
            {   //displayRecord(r);

                    if(event.key.code==sf::Keyboard::Right)
                    {
                        float rot=number_of_right_rotations*rotation;
                        cout<<"\ntotal rotation took place is->" <<rot;
                        rot=rot/720.0;
                        rot=rot+0.5; //making sure rot value lies between 0.5 - 1 denoting clockwise rotation
                        if(flag==1)
                        {   cout<<"\nflagrotate deactivated";
                            r->rotations=rot;
                            flag=0;
                            number_of_right_rotations=0;
                        }
                        cout<<"New angle is set and it is->"<<car.getRotation()<<endl;

                    }
                    if(event.key.code==sf::Keyboard::Left)
                    {

                        float rot=number_of_left_rotations*rotation;
                        cout<<"\ntotal rotation took place is->" <<rot;
                        rot=rot/720.0;
                         //making sure rot value lies between 0 - 0.5 denoting clockwise rotation

                        if(flag==1)
                        {   cout<<"\nflagrotate deactivated";
                            r->rotations=rot;
                            flag=0;
                            number_of_left_rotations=0;
                        }
                        cout<<"New angle is set and it is->"<<car.getRotation()<<endl;
                    }
                 /*   if(event.key.code==sf::Keyboard::Up)
                    {

                        if(flagMove==1)
                        {    cout<<"\nflagmove deactivated in up";
                            float upmoves=(number_of_up)/2*maxupdown;
                            number_of_up=0;
                            flagMove=0;
                            upmoves+=0.5; // to make sure upmoves are greater than 0.5
                            //now to make sure it is less than or equal to 1, we do
                            if(upmoves>1)
                            upmoves=1;
                            r->up=upmoves;
                        }
                    }
                    if(event.key.code==sf::Keyboard::Down)
                    {

                        if(flagMove==1)
                        {   cout<<"\nflagmove deactivated in down";
                            float downmoves=(number_of_down)/2*maxupdown;
                            number_of_down=0;
                            flagMove=0;

                            // now to make sure, down moves is <=0.5
                            if(downmoves>0.5)
                            downmoves=0.5;
                            r->up=downmoves;
                        }
                    }*/


            }

        }



    }
}

