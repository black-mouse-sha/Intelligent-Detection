// DHT11.ino
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

float temperature = 0.0;
float humidity = 0.0;
int tempThreshold = 30;   // 温度报警阈值（摄氏度）
int humThreshold = 80;    // 湿度报警阈值（百分比）

void setupDHT11() {
  dht.begin();
}

void readDHT11() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // 检查读取是否成功，失败则保持上次值
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT11!");
  }
}

void setTempThreshold(int val) { tempThreshold = val; }
void setHumThreshold(int val) { humThreshold = val; }