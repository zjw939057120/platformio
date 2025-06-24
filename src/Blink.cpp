/*
 * Blink
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */

#include <Arduino.h>
#include <STM32FreeRTOS.h>
#include <Adafruit_NeoPixel.h>

// 定义LED灯引脚
#ifdef ARDUINO_GENERIC_STM32F103RC
#define LED_BUILTIN_1 PC7
#define LED_BUILTIN_2 PC6
#elif ARDUINO_GENERIC_STM32F103VE
#define LED_BUILTIN_1 PB14
#define LED_BUILTIN_2 PB13
#endif

// 定义CAN缓冲区长度
#define CAN_BUFFER_LEN 13

#define NUM_CHN_MAX 8       // 定义最大通道数
#define NUM_PIXELS_MAX 1024 // 定义最大像素数量
// 定义LED灯带引脚和数量
#define PIN_PIXELS_0 PC0
#define NUM_PIXELS_0 64

#define PIN_PIXELS_1 PC1
#define NUM_PIXELS_1 64

#define PIN_PIXELS_2 PC2
#define NUM_PIXELS_2 64

#define PIN_PIXELS_3 PC3
#define NUM_PIXELS_3 64

#define PIN_PIXELS_4 PA4
#define NUM_PIXELS_4 64

#define PIN_PIXELS_5 PA5
#define NUM_PIXELS_5 64

#define PIN_PIXELS_6 PA6
#define NUM_PIXELS_6 64

#define PIN_PIXELS_7 PA7
#define NUM_PIXELS_7 64

#define DELAYVAL 500
// 定义LED灯带颜色数组
const uint32_t rainbowColors16[16] = {
    Adafruit_NeoPixel::Color(255, 0, 0),   // 红
    Adafruit_NeoPixel::Color(255, 64, 0),  // 红橙
    Adafruit_NeoPixel::Color(255, 128, 0), // 橙
    Adafruit_NeoPixel::Color(255, 191, 0), // 黄橙
    Adafruit_NeoPixel::Color(255, 255, 0), // 黄
    Adafruit_NeoPixel::Color(191, 255, 0), // 黄绿
    Adafruit_NeoPixel::Color(128, 255, 0), // 亮绿
    Adafruit_NeoPixel::Color(64, 255, 0),  // 绿黄
    Adafruit_NeoPixel::Color(0, 255, 0),   // 绿
    Adafruit_NeoPixel::Color(0, 255, 64),  // 青绿
    Adafruit_NeoPixel::Color(0, 255, 128), // 青
    Adafruit_NeoPixel::Color(0, 255, 191), // 蓝青
    Adafruit_NeoPixel::Color(0, 255, 255), // 蓝
    Adafruit_NeoPixel::Color(0, 128, 255), // 靛蓝
    Adafruit_NeoPixel::Color(64, 0, 255),  // 蓝紫
    Adafruit_NeoPixel::Color(128, 0, 255)  // 紫
};

// 定义LED灯带颜色数组
const uint32_t rainbowColors32[32] = {
    Adafruit_NeoPixel::Color(255, 0, 0), // 红
    Adafruit_NeoPixel::Color(255, 32, 0),
    Adafruit_NeoPixel::Color(255, 64, 0),
    Adafruit_NeoPixel::Color(255, 96, 0),
    Adafruit_NeoPixel::Color(255, 128, 0), // 橙
    Adafruit_NeoPixel::Color(255, 160, 0),
    Adafruit_NeoPixel::Color(255, 191, 0),
    Adafruit_NeoPixel::Color(255, 223, 0),
    Adafruit_NeoPixel::Color(255, 255, 0), // 黄
    Adafruit_NeoPixel::Color(191, 255, 0),
    Adafruit_NeoPixel::Color(128, 255, 0),
    Adafruit_NeoPixel::Color(64, 255, 0),
    Adafruit_NeoPixel::Color(0, 255, 0), // 绿
    Adafruit_NeoPixel::Color(0, 255, 32),
    Adafruit_NeoPixel::Color(0, 255, 64),
    Adafruit_NeoPixel::Color(0, 255, 96),
    Adafruit_NeoPixel::Color(0, 255, 128), // 青
    Adafruit_NeoPixel::Color(0, 255, 160),
    Adafruit_NeoPixel::Color(0, 255, 191),
    Adafruit_NeoPixel::Color(0, 255, 223),
    Adafruit_NeoPixel::Color(0, 255, 255), // 蓝
    Adafruit_NeoPixel::Color(0, 191, 255),
    Adafruit_NeoPixel::Color(0, 128, 255),
    Adafruit_NeoPixel::Color(0, 64, 255),
    Adafruit_NeoPixel::Color(0, 0, 255), // 靛蓝
    Adafruit_NeoPixel::Color(64, 0, 255),
    Adafruit_NeoPixel::Color(128, 0, 255),
    Adafruit_NeoPixel::Color(191, 0, 255),
    Adafruit_NeoPixel::Color(255, 0, 255), // 紫
    Adafruit_NeoPixel::Color(255, 0, 191),
    Adafruit_NeoPixel::Color(255, 0, 128),
    Adafruit_NeoPixel::Color(255, 0, 64)};

// 定义LED灯带对象
Adafruit_NeoPixel pixels_0(NUM_PIXELS_0, PIN_PIXELS_0, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_1(NUM_PIXELS_1, PIN_PIXELS_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_2(NUM_PIXELS_2, PIN_PIXELS_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_3(NUM_PIXELS_3, PIN_PIXELS_3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_4(NUM_PIXELS_4, PIN_PIXELS_4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_5(NUM_PIXELS_5, PIN_PIXELS_5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_6(NUM_PIXELS_6, PIN_PIXELS_6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels_7(NUM_PIXELS_7, PIN_PIXELS_7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel *m_pixels[8] = {&pixels_0, &pixels_1, &pixels_2, &pixels_3, &pixels_4, &pixels_5, &pixels_6, &pixels_7};

// 定义LED灯带模式结构体
// 用于存储LED灯带的颜色、亮度、效果、速度和模式
struct PixelMode
{
  uint16_t index;     // 索引
  uint16_t length;    // 长度
  uint32_t color;     // 颜色
  uint8_t mode;       // 模式
  uint8_t brightness; // 亮度
  uint8_t speed;      // 速度
};

// 定义LED灯带模式结构体
PixelMode pixelMode_0 = {0, NUM_PIXELS_0, rainbowColors16[0], 1, 32, 1};
PixelMode pixelMode_1 = {1, NUM_PIXELS_1, rainbowColors16[1], 1, 32, 1};
PixelMode pixelMode_2 = {2, NUM_PIXELS_2, rainbowColors16[2], 1, 32, 1};
PixelMode pixelMode_3 = {3, NUM_PIXELS_3, rainbowColors16[3], 1, 32, 1};
PixelMode pixelMode_4 = {4, NUM_PIXELS_4, rainbowColors16[4], 1, 32, 1};
PixelMode pixelMode_5 = {5, NUM_PIXELS_5, rainbowColors16[5], 1, 32, 1};
PixelMode pixelMode_6 = {6, NUM_PIXELS_6, rainbowColors16[6], 1, 32, 1};
PixelMode pixelMode_7 = {7, NUM_PIXELS_7, rainbowColors16[7], 1, 32, 1};
PixelMode *m_pixelMode[8] = {
    &pixelMode_0, &pixelMode_1, &pixelMode_2, &pixelMode_3,
    &pixelMode_4, &pixelMode_5, &pixelMode_6, &pixelMode_7};

// 定义FreeRTOS任务句柄
TaskHandle_t handleNeoPixel_0 = NULL;
TaskHandle_t handleNeoPixel_1 = NULL;
TaskHandle_t handleNeoPixel_2 = NULL;
TaskHandle_t handleNeoPixel_3 = NULL;
TaskHandle_t handleNeoPixel_4 = NULL;
TaskHandle_t handleNeoPixel_5 = NULL;
TaskHandle_t handleNeoPixel_6 = NULL;
TaskHandle_t handleNeoPixel_7 = NULL;

TaskHandle_t *m_handleNeoPixel[8] = {
    &handleNeoPixel_0, &handleNeoPixel_1, &handleNeoPixel_2, &handleNeoPixel_3,
    &handleNeoPixel_4, &handleNeoPixel_5, &handleNeoPixel_6, &handleNeoPixel_7};

// 定义FreeRTOS任务函数
void TaskBlink(void *pvParameters);
void TaskSerial(void *pvParameters);
void TaskNeoPixel(void *pvParameters);
// 灯带模式函数
void LightStripMode_0(uint8_t index);
void LightStripMode_1(uint8_t index);
void LightStripMode_2(uint8_t index);
void LightStripMode_3(uint8_t index);
void LightStripMode_4(uint8_t index);
void LightStripMode_5(uint8_t index);
void LightStripMode_6(uint8_t index);
void LightStripMode_7(uint8_t index);
void LightStripMode_8(uint8_t index);
void LightStripMode_9(uint8_t index);
void LightStripMode_10(uint8_t index);
void LightStripMode_11(uint8_t index);
void LightStripMode_12(uint8_t index);
void LightStripMode_13(uint8_t index);
void LightStripMode_14(uint8_t index);
void LightStripMode_15(uint8_t index);
void LightStripMode_255(uint8_t index);
void setup()
{
  // 初始化LED引脚
  pinMode(LED_BUILTIN_1, OUTPUT);
  pinMode(LED_BUILTIN_2, OUTPUT);
  // 初始化串口
  Serial.begin(115200);
  Serial.setTimeout(64); // 设置串口接收超时时间为5ms
  // 初始化LED灯带

  for (size_t i = 0; i < NUM_CHN_MAX; i++)
  {
    m_pixels[i]->begin();
    m_pixels[i]->setBrightness(m_pixelMode[i]->brightness); // 设置LED灯带亮度;
  }

  // 创建FreeRTOS任务
  xTaskCreate(TaskBlink, "Blink", 128, NULL, 2, NULL);
  xTaskCreate(TaskSerial, "Serial", 128, NULL, 2, NULL);
  xTaskCreate(TaskNeoPixel, "NeoPixel[0]", 128, m_pixelMode[0], 2, m_handleNeoPixel[0]);
  xTaskCreate(TaskNeoPixel, "NeoPixel[1]", 128, m_pixelMode[1], 2, m_handleNeoPixel[1]);
  xTaskCreate(TaskNeoPixel, "NeoPixel[2]", 128, m_pixelMode[2], 2, m_handleNeoPixel[2]);
  xTaskCreate(TaskNeoPixel, "NeoPixel[3]", 128, m_pixelMode[3], 2, m_handleNeoPixel[3]);
  xTaskCreate(TaskNeoPixel, "NeoPixel[4]", 128, m_pixelMode[4], 2, m_handleNeoPixel[4]);
  xTaskCreate(TaskNeoPixel, "NeoPixel[5]", 128, m_pixelMode[5], 2, m_handleNeoPixel[5]);
  xTaskCreate(TaskNeoPixel, "NeoPixel[6]", 128, m_pixelMode[6], 2, m_handleNeoPixel[6]);
  xTaskCreate(TaskNeoPixel, "NeoPixel[7]", 128, m_pixelMode[7], 2, m_handleNeoPixel[7]);
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
      uint8_t index = buffer[5]; // 获取索引值
      if (index >= NUM_CHN_MAX)
        break;                                                                               // 如果索引值超过最大通道数，退出任务
      m_pixelMode[index]->color = Adafruit_NeoPixel::Color(buffer[6], buffer[7], buffer[8]); // 更新颜色
      m_pixelMode[index]->mode = buffer[9];                                                  // 更新模式
      m_pixelMode[index]->speed = buffer[10];                                                // 更新速度
      m_pixelMode[index]->brightness = buffer[11];                                           // 更新亮度
      Serial.write(buffer, CAN_BUFFER_LEN);
      vTaskDelete(*m_handleNeoPixel[index]); // 删除当前任务
      xTaskCreate(TaskNeoPixel, "NeoPixel", 128, m_pixelMode[index], 2, m_handleNeoPixel[index]);
    }
  }
}

void TaskNeoPixel(void *pvParameters)
{
  PixelMode *pixelMode = (PixelMode *)pvParameters; // 获取任务参数中的索引值
  char index = pixelMode->index;                    // 获取索引值
  for (;;)
  {
    m_pixels[index]->clear();
    m_pixels[index]->setBrightness(m_pixelMode[index]->brightness);
    switch (m_pixelMode[index]->mode)
    {
    case 0:
      LightStripMode_0(index); // 全亮模式`
      break;
    case 1:
      LightStripMode_1(index); // 逐个亮模式
      break;
    case 2:
      LightStripMode_2(index); // 彩虹模式
      break;
    case 3:
      LightStripMode_3(index);
      break;
    case 4:
      LightStripMode_4(index);
      break;
    case 5:
      LightStripMode_5(index);
      break;
    case 6:
      LightStripMode_6(index);
      break;
    case 7:
      LightStripMode_7(index);
      break;
    case 8:
      LightStripMode_8(index);
      break;
    case 9:
      LightStripMode_9(index);
      break;
    case 10:
      LightStripMode_10(index);
      break;
    case 11:
      LightStripMode_11(index);
      break;
    case 12:
      LightStripMode_12(index);
      break;
    case 13:
      LightStripMode_13(index);
      break;
    case 14:
      LightStripMode_14(index);
      break;
    case 15:
      LightStripMode_15(index);
      break;
    case 255:
      LightStripMode_255(index); // 默认模式
      break;
    default:
      LightStripMode_255(index);
    }
  }
}

void LightStripMode_0(uint8_t index)
{
  // 全亮模式
  m_pixels[index]->fill(m_pixelMode[index]->color);
  m_pixels[index]->show();
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_1(uint8_t index)
{
  // 逐个亮模式
  for (int i = 0; i < m_pixelMode[index]->length; i++)
  {
    m_pixels[index]->setPixelColor(i, m_pixelMode[index]->color);
    m_pixels[index]->show();
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
  // vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_2(uint8_t index)
{
  // 彩虹模式
  for (int i = 0; i < m_pixelMode[index]->length / 32; i++)
  {
    for (size_t j = 0; j < 32; j++)
    {
      m_pixels[index]->setPixelColor(i * 32 + j, rainbowColors32[j]);
      m_pixels[index]->show();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  // vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_3(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_4(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_5(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_6(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_7(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_8(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_9(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_10(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_11(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_12(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_13(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_14(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_15(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}

void LightStripMode_255(uint8_t index)
{
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL); // 删除当前任务
}
