/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

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

#ifndef TENSORFLOW_LITE_MICRO_EXAMPLES_HELLO_WORLD_CONSTANTS_H_
#define TENSORFLOW_LITE_MICRO_EXAMPLES_HELLO_WORLD_CONSTANTS_H_

// Esta constante representa o intervalo de valores x em que nosso modelo foi treinado,
// que é de 0 a (2 * Pi). Aproximamos Pi para evitar a necessidade de
// bibliotecas.
const float kXrange = 2.f * 3.14159265359f;

// Essa constante determina o número de inferências a serem executadas no intervalo
// de x valores definidos acima. Como cada inferência leva tempo, quanto maior
// número, mais tempo levará para percorrer todo o intervalo. O valor que
// desta constante pode ser ajustado para que um ciclo completo leve uma quantidade desejada
// de tempo. Como diferentes dispositivos levam diferentes quantidades de tempo para executar
// inferência, este valor deve ser definido por dispositivo.
extern const int kInferencesPerCycle;

#endif  // TENSORFLOW_LITE_MICRO_EXAMPLES_HELLO_WORLD_CONSTANTS_H_