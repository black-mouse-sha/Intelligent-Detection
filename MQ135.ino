// ==================== MQ135.ino ====================
#define MQ135_PIN A3

int mq135Value = 0;
int mq135Threshold = 400;

void setupMQ135() {
  pinMode(MQ135_PIN, INPUT);
}

void readMQ135() {
  mq135Value = analogRead(MQ135_PIN);
}