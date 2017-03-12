#######################################################################################
Author:Saksham Jain
Email :ErSakshamJain@gmail.com
#######################################################################################
Introduction:
Following system helps you test your machine learning algorithm for autonomous navigation you can generate the dataset for Artificial neural networks
from the dataset you can train your neural network and furthur code is provided that uses the trained neural network which can be used for autonomous
navigation avoiding any collisions with obstacles.
#######################################################################################
Prerequisite:
1. Knowledge about basic game neural networks.
2. Installed sfml library for game programming.
3. Installed FANN library for using neural networks
#######################################################################################
Configuration:
Once above mentioned libraries are installed , if you want to use codeblocks, use the following configuration
go to settings compiler linker then in linker options add the following
-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
In order to link fann
go to settings compiler linker then in linker options add the following
-lfann
If you are on Ubuntu and you faced the following error
./main: error while loading shared libraries: libfann.so.2: cannot open shared object file: No such file or directory
Then run
sudo ldconfig
for command line
g++ -c main.cpp
then
g++ main.o -o -lsfml-graphics -lsfml-window -lsfml-system
you can also use the script provided just type ./script
change the file name you want compile
#######################################################################################
Dataset file format:
This software uses Artificial Neural networks, which need to be trained, input file format is as follows->
The first line consists of three numbers: The first is the number of training pairs in the file, the second is the number of inputs and the third is the number of outputs. The rest of the file is the actual training data, consisting of one line with inputs, one with outputs etc.
for ex.
4->number of examples
2->number of inputs per example
1->number of outputs per example
4 2 1
-1 -1
-1
-1 1
1
1 -1
1
1 1
-1
#######################################################################################
Information about various files included
1.generate_training_data.cpp- used to create dataset, a gui is presented, car (triangular shape_ with a field of view(blue circle attached to car) is there,
user can use up and down keys to move car in forward and backward direction. User should use up key to move car whenever an obstacle comes in field of view,
in command line its coordinates are shown after the line "obstacles in proximity->" once it happens user should press down the right or left key in order to rotate car and key should be released only once final direction is achieved. once this is done, one entry in dataset will be added.
2.data.txt - generated dataset by generate_training_data.cpp
3.fann_training_example.cpp- an example that shows how to use fann library in order to train neural networks
4.fann_execution_example.cpp- an example that shows how neural network eecutes after trained with dataset
5.script- basic linux script for compilation
6.sound.wav - current is not used , actually i wanted to make a sound whenever collision occurs for now that portion is broken.
7.xor.data- used by fann_training_example.cpp
8.xor_float.net- neural networks configuration file generated by fann_training_example.cpp
9.train_autonomous_robot.cpp- used to train our neural network which will be used for autonomous navigation, it uses the file data.txt as training data
10.robot_brain.net- configuration of neural network as obtained by train_autonomous_robot.cpp
11.simulate.cpp- uses robot_brain.net to simulate collision avoidance
#######################################################################################
Neural network architecture->
1.Back propagation neural networks are used.
2. 4 layers are there first layer has 2 inputs (left proximity, right proximity) . There are 2 hidden layers each having 7,8 hidden units and output layer tells the angle by which car should rotate(scaled angle)
3. if angle is >0.5 ,it should rotate in clock wise direction by an amount calculated as follows
	angle=angle-0.5;
	angle=angle*720;
	else if angle is <=0.5 it should rotate by an amount angle= angle*720;

Any suggestions and feedbacks are welcomed.
