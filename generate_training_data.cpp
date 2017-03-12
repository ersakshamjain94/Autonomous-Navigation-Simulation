//warnings
//always refer to field position with pos_circle variable a field is nothing but field of view of the car
// once you are done with training, close the gui window with upper right cross botton otherwise training data wont be saved
//it is assumed that user wont rotate more than 180 degrees
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include<math.h>
#include<stdlib.h>
#define rotation 5.0
#define step 5.0
#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#define placeholder 999 // a placeholder that is used so that, data.txt is only written with record when a valid record is available a valid record is one in which
//place holder is not there. after pushing the valid record, record is re initialized with placeholder
#define maxupdown 10 //used for scaling up and down moves
using namespace std;
//remember positions are defined by top left corner of rectangle covering any object always until we change it
//views are always modified by their centre

struct record {
    double left;  //distance from obstacle on left on a scale of 0-1 (1 representing no obstacle or very far obstacle)
    double front;
    double right;
    double rotations; //scaled rotations (0-1) a rotations >0.5 represents clockwise rotation . relative rotation angle can be calculated as
    // if rotations>0.5 then rotations=rotations-0.5 and rotations=rotations*720 else rotations=rotations*720
    double up; //accelration deaccelration although it is not used for feeding in neural network
};
class objects { //represents objects in field of view
   public:
    int side; //represent on which side is the object present in field of view side=0 (left) side=1 front side=2 right
    double distance; //represents distance from the object from the centre of car (scaled down to 0-1)
};
bool compareByDistance(const objects &a, const objects &b) //comparator used for sorting the objects according to their distances
{
    return a.distance < b.distance;
}
void displayRecord(struct record * r) //used to display current record which will be pushed into data.txt at later stages
{
    cout<<r->left<<" "<<r->front<<" "<<r->right<<" "<<r->up<<" "<<r->rotations<<endl;

}
void createObstacles(sf::CircleShape obstacle [],sf::Vector2u windowSize,int pointCount,int radius,int n) //sets up the objects within window size
{

        int i;
        int r,g,b;
        int x,y;
        srand(time(0));

        for(i=0;i<n;i++)
        {
            x=rand()%windowSize.x; //random location of objects
            y=rand()%windowSize.y;
           // cout<<"\nObstacle created at->"<<x<<" "<<y<<endl;
            r=rand()%255;
            g=rand()%255;   //random colors for objects
            b=rand()%255;
            sf::Color color(r,g,b);
            (obstacle[i]).setFillColor(color);
            (obstacle[i]).setRadius(radius);
            (obstacle[i]).setPointCount(pointCount);
            obstacle[i].setPosition(x,y);
            obstacle[i].setOrigin(radius,radius);

        }

}
vector<sf::CircleShape> getProximity(sf::CircleShape obstacle[],int num_obstacles,sf::Vector2f fieldPosition,float radius) //this function returns all the objects which are in proximity with field
{       vector<sf::CircleShape>result;
        float fieldx=fieldPosition.x;
        float fieldy=fieldPosition.y;

    int i;
    for(i=0;i<num_obstacles;i++)
    {   float obstaclex=obstacle[i].getPosition().x;
        float obstacley=obstacle[i].getPosition().y;
        double distance=(pow((obstaclex-fieldx),2)+pow((obstacley-fieldy),2)); //distance from centre of field to the centre of object
        distance=sqrt(distance);
        //cout<<"distance "<<distance<<endl; //uncomment to see distances of all objects
        if(distance<=radius)
        result.push_back(obstacle[i]); //obstacle i is in field of view
    }
    return result; //returns all the objects in close proximity to the field
}
struct record * printProximity(sf::CircleShape obstacle [],int num_obstacles,sf::Vector2f fieldPosition,sf::Vector2f carPosition,float radius,struct record * r,sf::Event event,sf::CircleShape car) //used to find out left and right proximity distances and fill in the record parameters for left and right
//record is nothing but one dataset record in the format leftdistance rightdistance \n rotation
{   FILE * fout=fopen("data.txt","a+");

    //http://math.stackexchange.com/questions/1435779/calculate-if-a-point-lies-above-or-below-or-right-to-left-of-a-line
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)||sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) //this block is only executed when car is moving
    {
     vector<sf::CircleShape>result=getProximity(obstacle,num_obstacles,fieldPosition,radius); //get the list of objects in proximity
    vector<sf::CircleShape> ::iterator it;
    cout<<"\nobjects in proximity are->"<<endl;
    //forming the equation of line joining carPosition and fieldPosition(x2,y2)
    double x1,x2,y1,y2;
    x1=carPosition.x; //car's current x coordinate
    y1=carPosition.y; //car's current y coordinate
    x2=fieldPosition.x; //field's curent x coordintate
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
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //1 represents forward direction 0 represents backward
        r->up=1;
    else
        r->up=0;
    int position;
    int finalposition=3;    //3 is just a placeholder
    vector<objects> obstaclesInRange;
        double dist;
    for(it=result.begin();it!=result.end();it++) //scan all the objects in the field of view
    {   struct objects singleobstacle;
        sf::CircleShape temp=*it;
        double x=temp.getPosition().x; //objects x coordinate
        double y=temp.getPosition().y; //also called Yp
        cout<<"X="<<x<<" "<<"y="<<y<<endl; //print objects coordinate in console only that object which is in proximity
        double value=(x2-x1)*(y-y1)-(y2-y1)*(x-x1); //just to calculate in which direction object lies
        if(value==0)
        position=1; //object lies in the front of car
        else if(value<0)
        position=0; //left of the line that is object is on the left side of car
        else
        position=2; //object lies on the right side of ccar
        singleobstacle.side=position; //setting the objects position relative to car (left,front,right)
        dist=pow((x-x1),2)+pow((y-y1),2); //calculating distance of object from the car's centre x1,y1 are car's centre coordinates
        dist=sqrt(dist);
        dist=dist/(2*radius+car.getRadius()); //used to scale distance between 0-1 radius is field of view's radius in worst case object can be
        //on the tip of field of view, that is vertical top of circle hence 2*radius distance from the tip of triangle and we are adding car's radius that is distance of centre and tip of car to it
        singleobstacle.distance=dist;
        obstaclesInRange.push_back(singleobstacle); //now all the obstacles that are in range along with their proximity details are combined in single vector

    }
    //std::sort(obstaclesInRange.begin(),obstaclesInRange.end(),compareByDistance);
    //now our main aim is to find out the closest left up and right object because only details of closest distances will be recorded in data.txt closest of all objects in proximity
        objects closestLeft,closestUp,closestRight; //we want to find these 3 objects in three directions
        closestLeft.distance=2; //default distance just a placeholder
        closestUp.distance=2;
        closestRight.distance=2;
        closestLeft.side=0; //setting direction to be left side
        closestUp.side=1;
        closestRight.side=2; //setting direction to the right
       vector<objects>::iterator objectsIterator;
        for(objectsIterator=obstaclesInRange.begin();objectsIterator!=obstaclesInRange.end();objectsIterator++) //for finding closest objects
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

        if(closestLeft.distance<=1) //if an object on the left side exists (only possible if placeholder 2 was updated and distance was scaled_
        {
            r->left=closestLeft.distance; //add to the record
        }
        if(closestRight.distance<=1) //checks if there is any object to the right
        {
            r->right=closestRight.distance;
        }
         if(closestUp.distance<=1)
        {
            r->front=closestUp.distance;
        }
        if(r->left>1) //if there is no object on a particular direction r->left etc. are by default initialized with 999 to represent no object we set it to 1
        r->left=1;
        if(r->front>1)
        r->front=1;
        if(r->right>1)
        r->right=1;
         cout<<"\nClosest distances are->"<<closestLeft.distance<<" "<<closestUp.distance<<" "<<closestRight.distance;
    }

        return r;
}
using namespace std;
int main()
{   //pos_circle is the position coordinate of field
    FILE * fout= fopen("data.txt","a+");
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
        cout<<"obstacle "<<i<<" at "<<"x="<<temp.x<<"y="<<temp.y<<endl; //prints all the object locations
    }
    sf::FloatRect obstacleRect[1000];
    for(i=0;i<number_obstacle;i++)
    {
        obstacleRect[i]=obstacle[i].getGlobalBounds(); //getting a rect for each obstacle
    }
    sf::FloatRect carRect=car.getGlobalBounds();//getting a bounding rectangle for car;
    sf::FloatRect fieldRect=field.getGlobalBounds(); //getting bouding rectangle for the field
   /* sf::CircleShape p1(1);
    p1.setPosition(car.getOrigin()); //if you uncomment this block, the points of origin and position will be highlighted for car and field
    cout<<"\np1="<<p1.getPosition().x<<" "<<p1.getPosition().y;
    p1.setFillColor(sf::Color::Magenta);
    sf::CircleShape p2(1);
    p2.setPosition(field.getOrigin());
    p2.setFillColor(sf::Color::White);
    cout<<"\np2="<<p2.getPosition().x<<" "<<p2.getPosition().y;
*/

     struct record * r = (struct record *)malloc(sizeof(struct record)); //record that will be pushed to data.txt
    r->front=placeholder; //initialized with placeholder
    r->left=placeholder;
    r->right=placeholder;
    r->up=placeholder;
    r->rotations=placeholder;
    float number_of_right_rotations=0; //we are actually calculating number of rotation steps required for the required turn
                                        //you can change rotation steps in the #define above it is named rotations
    float number_of_left_rotations=0;   //we are doing so for the purpose of calculating relative angles since sfml doest support relative angles directly
    float number_of_up=0; //not really used for now will be used in future purpose
    float number_of_down=0;
     int flag=0;
     int flagMove=0;
while(window.isOpen())
{       //while the window is open, this loop will continue executing
    window.clear(sf::Color::Black); //sets background color and deletes everything which was on window earlier
    window.draw(car); //draws but doesnt display
    sf::Transform fieldTransform=car.getTransform(); //copies transformation of car to the field so that field moves along with car

    window.draw(field,fieldTransform); //just shows field as if its moving, its position is fixed not moving it just appears to be moving
    //followint two lines are very important please dont change them
    sf::Vector2f fieldpositionwithoutoffset(0+field.getRadius(),0-field.getRadius());
    sf::Vector2f pos_circle=fieldTransform.transformPoint(fieldpositionwithoutoffset); //we are calculating position of field expicitely

    // window.draw(p1); //uncomment to check origins of field and car
    // window.draw(p2);
    for(i=0;i<number_obstacle;i++)
    {window.draw(obstacle[i]);
      sf::Vertex v(obstacle[i].getPosition()); //this segment of code is to draw a point at each position of the obstacle
        window.draw(&v, 1, sf::Points); //drawing the obstacles on the screen
    }
    sf::Vertex carPosvertex(car.getPosition()); //this is actually the little dot u see in the centre of car and field
    sf::Vertex fieldPosvertex(pos_circle); //pos_circle is the position of field (adjusted so that it moves along with car)
    window.draw(&carPosvertex,1,sf::Points);
    window.draw(&fieldPosvertex,1,sf::Points);
    window.display();
    sf::Event event;
        //flag used to check whtehr we are rotating because of obstacle or just like that
        // 0 represents just like that , 1 represents obstacle is there

        while(window.pollEvent(event))
        {   //an event occurs whenever we press a key
        //we write to a file only if record doesnt have any more place holders and notive we are only writing left,right,rotations for now for future improvement purpose i have
        //also added up and front just for future purpose we are actually setting up and front to 0 or 1 for now expicitely
            if(r->front!=placeholder && r->left!= placeholder && r->right != placeholder && r->rotations != placeholder && r->up != placeholder)
                {   printf("writing to the file %lf %lf %lf %lf %lf\n",r->left,r->front,r->right,r->up,r->rotations);
                    fprintf(fout,"%lf %lf\n%lf\n",r->left,r->right,r->rotations);
                    r->front=placeholder;
                    r->left=placeholder;
                    r->right=placeholder;
                    r->up=placeholder;
                    r->rotations=placeholder;

                }
            if(event.type==sf::Event::Closed) //closes window once upper right cross button is pressed
            {window.close();
             fclose(fout);
             }
            if(event.type==sf::Event::KeyPressed) //while any keyboard key is pressed or kept pressed this will execute
            {    vector<sf::CircleShape>result=getProximity(obstacle,number_obstacle,pos_circle,field.getRadius()); //field position(pos_circle_ and its radius is passed
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
                    number_of_right_rotations++; //every press of key generates one event hence increment it for each right button pressed, we will reset it to zero after we release the key and record the total number of rotations later multiply it to get relative angle
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
                 y = step*(float)cos((M_PI / 180.0f) * (360.0f-car.getRotation())); //moving with the help of polar system in desired direction
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

            if(event.type==sf::Event::KeyReleased) //whenever keyboard key is released , its time to calculate relative angles
            {   //displayRecord(r);

                    if(event.key.code==sf::Keyboard::Right)
                    {   //scaling between -1 to 1
                        double rot=number_of_right_rotations*rotation; //calculate relative angle 'rotation' is just one step of rotation
                        cout<<"\ntotal rotation took place is->" <<rot;
                        rot=(2*(rot-(-180.0f))/360.0)-1; //feature scaling
                        //rot=rot+0.5; //making sure rot value lies between 0.5 - 1 denoting clockwise rotation
                        if(flag==1) //flag is only set when we start calculating number of rotation sets once relative angle is calculated deactivate it
                        {   cout<<"\nflagrotate deactivated";
                            r->rotations=rot;
                            flag=0;
                            number_of_right_rotations=0;
                        }
                        cout<<"\nrot->"<<rot;
                        cout<<"New angle is set and it is->"<<car.getRotation()<<endl;

                    }
                    if(event.key.code==sf::Keyboard::Left)
                    {
                    //scaling into range http://stackoverflow.com/questions/5294955/how-to-scale-down-a-range-of-numbers-with-a-known-min-and-max-value
                    //range -1 to 1
                        double rot=-1*(number_of_left_rotations*rotation);
                        cout<<"\ntotal rotation took place is->" <<rot;
                        rot=(2*(rot-(-180.0f))/360.0)-1;
                        cout<<"\nrot->"<<rot;
                         //making sure rot value lies between 0 - 0.5 denoting clockwise rotation

                        if(flag==1)
                        {   cout<<"\nflagrotate deactivated";
                            r->rotations=rot;
                            flag=0;
                            number_of_left_rotations=0;
                        }
                        cout<<"New angle is set and it is->"<<car.getRotation()<<endl;
                    }
                 /*   if(event.key.code==sf::Keyboard::Up) //this block will evolve in future to add up and down features to the neural net
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

