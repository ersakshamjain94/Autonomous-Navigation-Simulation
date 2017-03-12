#include<fann.h>
#include<iostream>
using namespace std;
int main()
{
unsigned int layers[4] = {2, 8, 9, 1}; //2 input layers, 8 neurons in first hidden layer , 9 neurons in second hidden layer ,1 neuron in output layer
struct fann *ann = fann_create_standard_array(4, layers);
 const float desired_error = (const float) 0.001;
    const unsigned int max_epochs = 500000;
    const unsigned int epochs_between_reports = 1000;
    fann_set_activation_function_hidden(ann, FANN_SIGMOID);
    fann_set_activation_function_output(ann, FANN_SIGMOID);
    fann_train_on_file(ann, "data.txt", max_epochs,
        epochs_between_reports, desired_error);
        fann_save(ann, "robot_brain.net");
         fann_destroy(ann);

    return 0;
}
