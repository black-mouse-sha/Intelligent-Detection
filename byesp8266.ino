// oldwifi.ino
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <SoftwareSerial.h>

#define LIGHT_ALARM_WHEN 200
#define BUZZER_PIN 3
#define LED_PIN 5

// 全局变量声明（实际定义在各模块中）
extern int mq2Threshold, mq2Value;
extern int lightThreshold, lightValue;
extern int tempThreshold, humThreshold;
extern float temperature, humidity;

// 新增模块变量声明
extern int dustValue, dustThreshold;
extern int mq135Value, mq135Threshold;
extern int mq7Value, mq7Threshold;
extern int turbidityValue, turbidityThreshold;

// 按键调节模式（在主文件中定义）
int adjustMode = 0;  // 0:无, 1:MQ2, 2:光敏, 3:温度, 4:湿度, 5:粉尘, 6:MQ135, 7:MQ7, 8:浊度

void setup() {
  Serial.begin(115200);
  
  // 原有模块初始化
  setupOLED();
  setupDHT11();
  setupMQ2();
  setupLDR();
  setupBuzzer();
  setupButton();
  setupLED();
  setupESP8266();

  // 新增模块初始化
  setupDust();
  // setupMQ135();
  // setupMQ7();
  setupRelay();
  setupTurbidity();

  displayMessage("System Ready", 200);
}

void loop() {
  // 读取所有传感器
  readMQ2();
  readDHT11();
  readLDR();
  readDust();
  // readMQ135();
  // readMQ7();
  readTurbidity();

  handleButton();      // 按键处理（阈值调节）
  updateDisplay();     // 刷新OLED显示
  checkAlarms();       // 报警逻辑
  sendDataViaWiFi();   // WiFi发送数据

  delay(200);
}





// 报警逻辑
void checkAlarms() {
  bool alarm = false;
  
  if (mq2Value >= mq2Threshold) alarm = true;
  if (lightValue > lightThreshold) alarm = true;
  if (dustValue >= dustThreshold) alarm = true;
  // if (mq135Value == mq135Threshold) alarm = true;
  // if (mq7Value == mq7Threshold) alarm = true;
  if (turbidityValue >= turbidityThreshold) alarm = true;
  if (temperature >= tempThreshold) alarm = true;
  if (humidity >= humThreshold) alarm = true;

  if (alarm) {
    digitalWrite(BUZZER_PIN, HIGH);
    digitalWrite(LED_PIN, HIGH);
    relayOn();      // 可选：触发外部设备
  } else {
    digitalWrite(BUZZER_PIN, LOW);
    digitalWrite(LED_PIN, LOW);
    relayOff();
  }
}