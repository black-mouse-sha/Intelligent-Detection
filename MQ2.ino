// MQ2.ino
#define MQ2_PIN A0

int mq2Threshold = 300;  // 默认阈值
int mq2Value = 0;

void setupMQ2() {
  pinMode(MQ2_PIN, INPUT);
}

void readMQ2() {
  mq2Value = analogRead(MQ2_PIN);
}

// 可由外部调用的阈值设置函数
void setMQ2Threshold(int val) {
  mq2Threshold = val;
}