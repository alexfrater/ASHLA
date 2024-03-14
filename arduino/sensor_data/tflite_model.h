#include "model_data.cc" // Include the model data


// #define NUMBER_OF_INPUTS (140*61) //8540   // replace 'x' with the actual number of inputs for your model
// #define NUMBER_OF_OUTPUTS 3 // replace 'y' with the actual number of outputs of your model
// #define TENSOR_ARENA_SIZE (256 * 1024)  // 256 KB  // replace 'z' with the size in bytes that works for your model

// EloquentTinyML tflite(model_data, TENSOR_ARENA_SIZE);
// float inputs[NUMBER_OF_INPUTS];
// float outputs[NUMBER_OF_OUTPUTS];

void setup() {
    Serial.begin(9600);

    while (!Serial); // Wait for the serial monitor

    Serial.println("Setting up the model...");
    tflite.begin(inputs, outputs);
}

void loop() {
    // Fill 'inputs' with sensor data or other input

    // Run the model on the input data
    tflite.predict();

    // Use 'outputs' to get the model's predictions
    for (size_t i = 0; i < NUMBER_OF_OUTPUTS; i++) {
        Serial.print("Output ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(outputs[i], 6); // print the output with 6 decimal places
    }

    // Delay for a second for readability
    delay(1000);
}



