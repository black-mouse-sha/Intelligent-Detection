// ==================== SoilMoisture.ino ====================
#define SOIL_PIN A5

int soilValue = 0;
int soilThreshold = 500;   // 数值越大表示越干燥

void setupSoil() {
  pinMode(SOIL_PIN, INPUT);
}

void readSoil() {
  soilValue = analogRead(SOIL_PIN);
}