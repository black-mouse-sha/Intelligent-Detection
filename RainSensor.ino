// ==================== RainSensor.ino ====================
#define RAIN_PIN A5

int rainValue = 0;
int rainThreshold = 300;   // 数值越小表示雨滴越多

void setupRain() {
  pinMode(RAIN_PIN, INPUT);
}

void readRain() {
  rainValue = analogRead(RAIN_PIN);
}