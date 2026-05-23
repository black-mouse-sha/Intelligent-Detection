// ==================== DustSensor.ino ====================
#define DUST_PIN A2
#define DUST_LED_PIN 6

int dustValue = 0;
int dustThreshold = 150;

void setupDust() {
  pinMode(DUST_LED_PIN, OUTPUT);
  digitalWrite(DUST_LED_PIN, LOW);
}

void readDust() {
  digitalWrite(DUST_LED_PIN, HIGH);
  delayMicroseconds(280);
  dustValue = analogRead(DUST_PIN);
  delayMicroseconds(40);
  digitalWrite(DUST_LED_PIN, LOW);
  delayMicroseconds(9680);
}