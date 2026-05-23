// ==================== MQ7.ino ====================
#define MQ7_PIN A4

int mq7Value = 0;
int mq7Threshold = 300;

void setupMQ7() {
  pinMode(MQ7_PIN, INPUT);
}

void readMQ7() {
  mq7Value = analogRead(MQ7_PIN);
}