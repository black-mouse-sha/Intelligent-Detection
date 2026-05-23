// ==================== Turbidity.ino ====================
#define TURBIDITY_PIN A5

int turbidityValue = 0;
int turbidityThreshold = 500;   // 数值越大表示水越浑浊

void setupTurbidity() {
  pinMode(TURBIDITY_PIN, INPUT);
}

void readTurbidity() {
  turbidityValue = analogRead(TURBIDITY_PIN);
}