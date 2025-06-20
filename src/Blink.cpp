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

#define PIN PA8
#define NUMPIXELS 32

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

void TaskBlink(void *pvParameters)
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN_1, OUTPUT);
  pinMode(LED_BUILTIN_2, OUTPUT);

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

void TaskNeoPixel(void *pvParameters)
{
  pixels.begin();

  for (;;)
  {
    pixels.clear();
    for (int i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, pixels.Color(0, 150, 0));
      pixels.show();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(5); // 设置串口接收超时时间为5ms

  xTaskCreate(TaskBlink, "Blink", 128, NULL, 1, NULL);
  xTaskCreate(TaskNeoPixel, "NeoPixel", 128, NULL, 2, NULL);
  vTaskStartScheduler();
}

void loop()
{
  uint8_t buffer[CAN_BUFFER_LEN] = {0};
  if (Serial.available() > 0)
  {
    size_t len = Serial.readBytes(buffer, CAN_BUFFER_LEN);
    Serial.write(buffer, CAN_BUFFER_LEN);
  }
}
