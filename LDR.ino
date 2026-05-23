// LDR.ino - 适配接 AO 口的光敏传感器模块
#define LDR_PIN A1            // 光敏传感器 AO 口连接到模拟引脚 A1

int lightValue = 0;           // 存储模拟读数 (0~1023)
int lightThreshold = 500;     // 光照报警阈值（可根据实际环境调整）

void setupLDR() {
  pinMode(LDR_PIN, INPUT);    // 模拟引脚默认就是输入，这句可省略，但保留无妨
}

void readLDR() {
  lightValue = analogRead(LDR_PIN);  // 读取模拟值
}

void setLightThreshold(int val) {
  lightThreshold = val;               // 允许外部调整阈值
}