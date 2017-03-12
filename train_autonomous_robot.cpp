#include<fann.h>
#include<iostream>
using namespace std;
int main()
{
unsigned int layers[5] = {2, 9, 9,9, 1}; //2 input layers, 8 neurons in first hidden layer , 9 neurons in second hidden layer ,1 neuron in output layer
struct fann *ann = fann_create_standard_array(5, layers); //crates neural network
 const float desired_error = (const float) 0.001; //desired error
    const unsigned int max_epochs = 500000; //number of cycles , training is repeated
    const unsigned int epochs_between_reports = 1000; //report generated after every 1000 epochs
    fann_set_activation_function_hidden(ann, FANN_SIGMOID); //0-1 sigmoid function is used for both hidden and output layers
    fann_set_activation_function_output(ann, FANN_SIGMOID);
    fann_train_on_file(ann, "data.txt", max_epochs,
        epochs_between_reports, desired_error); //data.txt is used as a training file in prescribed format
        fann_save(ann, "robot_brain.net"); //save neural net specifications in a file
         fann_destroy(ann);

    return 0;
}
