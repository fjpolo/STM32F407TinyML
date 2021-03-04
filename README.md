# TinyML using different frameworks applied to STM32F407 uC

## Caution:
CubeMX code must be generated for every project, since libraries are included in .gitignore. For TFLite projects, the auto-generated main must be changed to .cpp and must be merged with the latest changes from main.c.-

Development IDE: Keil uVision v6

## CubeMXAI/003MagicWand

Modify the following parameters in CubeMXAI\003MagicWand\Middlewares\Third_Party\MachineLearning\edgeimpulse\model-parameters\model_metadata.h:
#define EI_CLASSIFIER_RAW_SAMPLE_COUNT           200
#define EI_CLASSIFIER_INTERVAL_MS                10
#define EI_CLASSIFIER_FREQUENCY                  100

Disable the following files for compilation:
- BasicMathFunctionsF16.c
- CommoTablesF16.c
- TransformFunctionsF16.c

## References:
- https://www.digikey.ph/en/maker/projects/intro-to-tinyml-part-1-training-a-model-for-arduino-in-tensorflow/8f1fc8c0b83d417ab521c48864d2a8ec
- https://colab.research.google.com/github/tensorflow/tensorflow/blob/master/tensorflow/lite/micro/examples/hello_world/train/train_hello_world_model.ipynb#scrollTo=l4-WhtGpvb-E
- https://www.digikey.com/en/maker/projects/tinyml-getting-started-with-tensorflow-lite-for-microcontrollers/c0cdd850f5004b098d263400aa294023
- https://www.digikey.com/en/maker/projects/tinyml-getting-started-with-stm32-x-cube-ai/f94e1c8bfc1e4b6291d0f672d780d2c0
- https://github.com/tensorflow/tensorflow/issues/45686#issuecomment-789658362
- https://www.coursera.org/learn/introduction-to-embedded-machine-learning
- https://studio.edgeimpulse.com/
- https://github.com/RashadShubita/STM32F407-tinyML-EdgeImpulse-Motion
- https://github.com/BoschSensortec/BNO055_driver
- https://github.com/ivyknob/bno055_stm32
- https://www.keil.com/pack/doc/compiler/EventRecorder/html/er_use.html

## ToDo:
- https://github.com/choas/uTensor_workshop
