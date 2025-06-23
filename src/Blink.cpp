/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <Adafruit_NeoPixel.h>

#ifdef ARDUINO_GENERIC_STM32F103RC
#define LED_BUILTIN_1 PC7
#define LED_BUILTIN_2 PC6
#elif ARDUINO_GENERIC_STM32F103VE
#define LED_BUILTIN_1 PB14
#define LED_BUILTIN_2 PB13
#endif

#define CAN_BUFFER_LEN 13

#define PIN_PIXELS_1 PC0
#define NUM_PIXELS_1 64

#define PIN_PIXELS_2 PC1
#define NUM_PIXELS_2 64

#define PIN_PIXELS_3 PC2
#define NUM_PIXELS_3 64

#define PIN_PIXELS_4 PC3
#define NUM_PIXELS_4 64

#define PIN_PIXELS_5 PA4
#define NUM_PIXELS_5 64

#define PIN_PIXELS_6 PA5
#define NUM_PIXELS_6 64

#define PIN_PIXELS_7 PA6
#define NUM_PIXELS_7 64

#define PIN_PIXELS_8 PA7
#define NUM_PIXELS_8 64

Adafruit_NeoPixel pixels_1(NUM_PIXELS_1, PIN_PIXELS_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_2(NUM_PIXELS_2, PIN_PIXELS_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_3(NUM_PIXELS_3, PIN_PIXELS_3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_4(NUM_PIXELS_4, PIN_PIXELS_4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_5(NUM_PIXELS_5, PIN_PIXELS_5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_6(NUM_PIXELS_6, PIN_PIXELS_6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_7(NUM_PIXELS_7, PIN_PIXELS_7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_8(NUM_PIXELS_8, PIN_PIXELS_8, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

void TaskBlink(void *pvParameters);
void TaskSerial(void *pvParameters);
void TaskNeoPixel_1(void *pvParameters);
void TaskNeoPixel_2(void *pvParameters);
void TaskNeoPixel_3(void *pvParameters);
void TaskNeoPixel_4(void *pvParameters);
void TaskNeoPixel_5(void *pvParameters);
void TaskNeoPixel_6(void *pvParameters);
void TaskNeoPixel_7(void *pvParameters);
void TaskNeoPixel_8(void *pvParameters);
void setup()
{
  // 初始化LED引脚
  pinMode(LED_BUILTIN_1, OUTPUT);
  pinMode(LED_BUILTIN_2, OUTPUT);
  // 初始化串口
  Serial.begin(115200);
  Serial.setTimeout(64); // 设置串口接收超时时间为5ms
  // 初始化LED灯带
  pixels_1.begin();
  pixels_1.setBrightness(8); // 设置LED灯带亮度;

  pixels_2.begin();
  pixels_2.setBrightness(8); // 设置LED灯带亮度;

  pixels_3.begin();
  pixels_3.setBrightness(8); // 设置LED灯带亮度;

  pixels_4.begin();
  pixels_4.setBrightness(8); // 设置LED灯带亮度;

  pixels_5.begin();
  pixels_5.setBrightness(8); // 设置LED灯带亮度;

  pixels_6.begin();
  pixels_6.setBrightness(8); // 设置LED灯带亮度;

  pixels_7.begin();
  pixels_7.setBrightness(8); // 设置LED灯带亮度;

  pixels_8.begin();
  pixels_8.setBrightness(8); // 设置LED灯带亮度;

  // 创建FreeRTOS任务
  xTaskCreate(TaskBlink, "Blink", 128, NULL, 1, NULL);
  xTaskCreate(TaskSerial, "Serial", 128, NULL, 1, NULL);
  xTaskCreate(TaskNeoPixel_1, "NeoPixel_1", 128, NULL, 2, NULL);
  xTaskCreate(TaskNeoPixel_2, "NeoPixel_2", 128, NULL, 2, NULL);
  xTaskCreate(TaskNeoPixel_3, "NeoPixel_3", 128, NULL, 2, NULL);
  xTaskCreate(TaskNeoPixel_4, "NeoPixel_4", 128, NULL, 2, NULL);
  xTaskCreate(TaskNeoPixel_5, "NeoPixel_5", 128, NULL, 2, NULL);
  xTaskCreate(TaskNeoPixel_6, "NeoPixel_6", 128, NULL, 2, NULL);
  xTaskCreate(TaskNeoPixel_7, "NeoPixel_7", 128, NULL, 2, NULL);
  xTaskCreate(TaskNeoPixel_8, "NeoPixel_8", 128, NULL, 2, NULL);
  vTaskStartScheduler();
}

void loop()
{
  // FreeRTOS任务调度器将接管控制权，loop函数将不会被调用
  // 所有的任务将在各自的函数中运行
  // 如果没有创建任何任务，loop函数将会一直空转
  // 因此在FreeRTOS环境下，loop函数通常不需要实现任何内容
  // 但为了保持代码完整性，这里保留一个空的loop函数
  // 以防万一需要在将来添加其他功能
  delay(1000); // 保持loop函数不为空，避免编译器优化掉
}

void TaskBlink(void *pvParameters)
{
  for (;;)
  {
    digitalWrite(LED_BUILTIN_1, HIGH);
    digitalWrite(LED_BUILTIN_2, LOW);
    // wait for a second
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    digitalWrite(LED_BUILTIN_1, LOW);
    digitalWrite(LED_BUILTIN_2, HIGH);
    // wait for a second
    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void TaskSerial(void *pvParameters)
{
  for (;;)
  {
    uint8_t buffer[CAN_BUFFER_LEN] = {0};
    if (Serial.available() > 0)
    {
      size_t len = Serial.readBytes(buffer, CAN_BUFFER_LEN);
      Serial.write(buffer, CAN_BUFFER_LEN);
    }
  }
}

void TaskNeoPixel_1(void *pvParameters)
{
  for (;;)
  {
    pixels_1.clear();
    for (int i = 0; i < NUM_PIXELS_1; i++)
    {
      pixels_1.setPixelColor(i, pixels_1.Color(255, 255, 255));
      pixels_1.show();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

void TaskNeoPixel_2(void *pvParameters)
{
  for (;;)
  {
    pixels_2.clear();
    for (int i = 0; i < NUM_PIXELS_2; i++)
    {
      pixels_2.setPixelColor(i, pixels_2.Color(255, 255, 255));
      pixels_2.show();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

void TaskNeoPixel_3(void *pvParameters)
{
  for (;;)
  {
    pixels_3.clear();
    for (int i = 0; i < NUM_PIXELS_3; i++)
    {
      pixels_3.setPixelColor(i, pixels_3.Color(255, 255, 255));
      pixels_3.show();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

void TaskNeoPixel_4(void *pvParameters)
{
  for (;;)
  {
    pixels_4.clear();
    for (int i = 0; i < NUM_PIXELS_4; i++)
    {
      pixels_4.setPixelColor(i, pixels_4.Color(255, 255, 255));
      pixels_4.show();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

void TaskNeoPixel_5(void *pvParameters)
{
  for (;;)
  {
    pixels_5.clear();
    for (int i = 0; i < NUM_PIXELS_5; i++)
    {
      pixels_5.setPixelColor(i, pixels_5.Color(255, 255, 255));
      pixels_5.show();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

void TaskNeoPixel_6(void *pvParameters)
{
  for (;;)
  {
    pixels_6.clear();
    for (int i = 0; i < NUM_PIXELS_6; i++)
    {
      pixels_6.setPixelColor(i, pixels_6.Color(255, 255, 255));
      pixels_6.show();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

void TaskNeoPixel_7(void *pvParameters)
{
  for (;;)
  {
    pixels_7.clear();
    for (int i = 0; i < NUM_PIXELS_7; i++)
    {
      pixels_7.setPixelColor(i, pixels_7.Color(255, 255, 255));
      pixels_7.show();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}
void TaskNeoPixel_8(void *pvParameters)
{
  for (;;)
  {
    pixels_8.clear();
    for (int i = 0; i < NUM_PIXELS_8; i++)
    {
      pixels_8.setPixelColor(i, pixels_8.Color(255, 255, 255));
      pixels_8.show();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}
