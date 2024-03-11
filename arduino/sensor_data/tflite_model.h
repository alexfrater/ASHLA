#include <compatibility.h>
#include <debug_log.h>
#include <fake_micro_context.h>
#include <flatbuffer_utils.h>
#include <memory_helpers.h>
#include <micro_allocation_info.h>
#include <micro_allocator.h>
#include <micro_arena_constants.h>
#include <micro_common.h>
#include <micro_context.h>
#include <micro_graph.h>
#include <micro_interpreter.h>
#include <micro_interpreter_context.h>
#include <micro_interpreter_graph.h>
#include <micro_log.h>
#include <micro_mutable_op_resolver.h>
#include <micro_op_resolver.h>
#include <micro_profiler.h>
#include <micro_profiler_interface.h>
#include <micro_resource_variable.h>
#include <micro_time.h>
#include <micro_utils.h>
#include <mock_micro_graph.h>
#include <recording_micro_allocator.h>
#include <recording_micro_interpreter.h>
#include <system_setup.h>
#include <test_helper_custom_ops.h>
#include <test_helpers.h>

#ifndef TFLITE_MODEL_H
#define TFLITE_MODEL_H

#include "model_data.cc" // Include the model data


// Define the tensor arena size. You might need to adjust this depending on your model's requirements and your device's memory constraints.
constexpr int kTensorArenaSize = 16 * 1024; // Example: 16 KB
byte tensorArena[kTensorArenaSize];

namespace {
  tflite::ErrorReporter* errorReporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* inputTensor = nullptr;
  TfLiteTensor* outputTensor = nullptr;
}

// Function to initialize the TensorFlow Lite model
void setupTFLiteModel() {
  static tflite::MicroErrorReporter microErrorReporter; // Static variable for error reporting.
  errorReporter = &microErrorReporter;
  
  model = tflite::GetModel(model_data);
  if (model->version() != TFLITE_SCHEMA_VERSION) {
    errorReporter->Report("Model version does not match TensorFlow Lite schema version.");
    return;
  }
  
  static tflite::AllOpsResolver resolver; // Static variable for the ops resolver.
  
  static tflite::MicroInterpreter staticInterpreter(model, resolver, tensorArena, kTensorArenaSize, errorReporter);
  interpreter = &staticInterpreter;

  // Allocate memory for tensors.
  TfLiteStatus allocateStatus = interpreter->AllocateTensors();
  if (allocateStatus != kTfLiteOk) {
    errorReporter->Report("Failed to allocate tensors.");
    return;
  }

  // Obtain pointers to the model's input and output tensors.
  inputTensor = interpreter->input(0);
  outputTensor = interpreter->output(0);
}

// Function to perform inference
void runInference() {
  // Ensure you've properly set the inputTensor data before calling this function.
  
  // Run the model on the input tensor and check for success.
  if (interpreter->Invoke() != kTfLiteOk) {
    errorReporter->Report("Failed to invoke TensorFlow Lite interpreter.");
    return;
  }

  // Here you can read the outputTensor data and process the inference results.
}

#endif // TFLITE_MODEL_H
