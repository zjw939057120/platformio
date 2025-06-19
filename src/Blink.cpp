/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <Adafruit_NeoPixel.h>

#ifdef ARDUINO_GENERIC_STM32F103RC
#define LED_BUILTIN PC7
#endif

#define PIN PA8
#define NUMPIXELS 32

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 500

void TaskBlink(void *pvParameters)
{
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  for (;;)
  {
    // turn the LED on (HIGH is the voltage level)
    digitalWrite(LED_BUILTIN, HIGH);
    // wait for a second
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    // turn the LED off by making the voltage LOW
    digitalWrite(LED_BUILTIN, LOW);
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
  xTaskCreate(TaskBlink, "Blink", 128, NULL, 1, NULL);
  xTaskCreate(TaskNeoPixel, "NeoPixel", 128, NULL, 2, NULL);
  vTaskStartScheduler();
}

void loop()
{
  // Empty. Things are done in Tasks.
}
