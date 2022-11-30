#include <TensorFlowLite_ESP32.h>
/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/


#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/system_setup.h"
#include "tensorflow/lite/schema/schema_generated.h"

#include "header/main_functions.h"
#include "header/model.h"
#include "header/constants.h"
#include "header/output_handler.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
tflite::ErrorReporter* error_reporter = nullptr;
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

constexpr int kTensorArenaSize = 2000;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace
const byte pot = 26;
const byte led = 4;
float y_return = 0;

// The name of this function is important for Arduino compatibility.
void setup() {

  Serial.begin(115200);

  ledcAttachPin(led,0); //canal 0
  ledcSetup (0,2000,10); // 10 bits

  
  static tflite::MicroErrorReporter micro_error_reporter;
  error_reporter = &micro_error_reporter;

    // Mapeie o modelo em uma estrutura de dados utilizável. 
    // Isso não envolve nenhuma cópia ou análise, é uma operação muito leve.
    model = tflite::GetModel(g_model);
    if (model->version() != TFLITE_SCHEMA_VERSION) {
      TF_LITE_REPORT_ERROR(error_reporter,
                          "Model provided is schema version %d not equal "
                          "to supported version %d.",
                          model->version(), TFLITE_SCHEMA_VERSION);

    }

   
    static tflite::AllOpsResolver resolver;

    // Build an interpreter to run the model with.
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
      TF_LITE_REPORT_ERROR(error_reporter, "AllocateTensors() failed");
      return;
    }

    // Obtain pointers to the model's input and output tensors.
    input = interpreter->input(0);
    output = interpreter->output(0);

    // Keep track of how many inferences we have performed.
    // Acompanhe quantas inferências realizamos.
    inference_count = 0;
  }

// The name of this function is important for Arduino compatibility.
void loop() {

float x_val_pi =  mapfloat(analogRead(pot), 0, 4095, 0.f, 6.283185482f);

 Serial.println();
 Serial.print("valor x em pi:");
 Serial.println(x_val_pi);
  


  
  int8_t x_quantized = x_val_pi / input->params.scale + input->params.zero_point;

  // Place the quantized input in the model's input tensor
  // inferir um valor X, a partir do potênciometro, de acordo com a escala do tensor

  input->data.int8[0] = x_quantized;

  // Run inference, and report an  ledcWrite(0, y_return*100);y error
  TfLiteStatus invoke_status = interpreter->Invoke();
  if (invoke_status != kTfLiteOk) {
    TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed on x: %f\n",
                         static_cast<double>(x_val_pi));
    return;
  }

  // Obtain the quantized output from model's output tensor
  int8_t y_quantized = output->data.int8[0];
  // Dequantize the output from integer to floating-point
  float y = (y_quantized - output->params.zero_point) * output->params.scale;
  
  // Output the results. A custom HandleOutput function can be implemented
  // for each supported hardware target.
  y_return = HandleOutput(error_reporter, x_val_pi, y);
  
  ledcWrite(0, mapfloat(y_return, -1.f, 1.f, 0, 1025));
  Serial.println();
  Serial.print("valor da saida:");
  Serial.println(y_return);
  
  delay(300);
}
