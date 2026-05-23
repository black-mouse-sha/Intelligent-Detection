// Button.ino
#define BUTTON_PIN 4

// 声明在其他文件中定义的全局变量
extern int adjustMode;
extern int mq2Threshold;
extern int lightThreshold;
extern int tempThreshold;
extern int humThreshold;
extern int dustThreshold;
extern int mq135Threshold;
extern int mq7Threshold;
extern int turbidityThreshold;

// 声明在其他文件中定义的函数
extern void buzzerBeep(int durationMs);

void setupButton() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void handleButton() {
  static int lastState = HIGH;
  static unsigned long pressStart = 0;
  static bool longPressHandled = false;
  int reading = digitalRead(BUTTON_PIN);

  // 按键状态变化时进行消抖
  if (reading != lastState) {
    delay(20);
    reading = digitalRead(BUTTON_PIN);
    if (reading != lastState) {
      if (reading == LOW) {                     // 按键按下
        pressStart = millis();
        longPressHandled = false;
      } else {                                  // 按键释放
        if (pressStart != 0 && !longPressHandled && (millis() - pressStart < 1000)) {
          // 短按：切换调节模式
          adjustMode++;
          if (adjustMode > 8) adjustMode = 1;
          buzzerBeep(50);
        }
        pressStart = 0;
      }
      lastState = reading;
    }
  }

  // 长按检测（按下超过 1 秒）
  if (lastState == LOW && pressStart != 0 && !longPressHandled && (millis() - pressStart > 1000)) {
    longPressHandled = true;
    // 根据当前调节模式增加对应阈值
    switch(adjustMode) {
      case 1: mq2Threshold += 10; break;
      case 2: lightThreshold += 10; break;
      case 3: tempThreshold += 1; break;
      case 4: humThreshold += 1; break;
      case 5: dustThreshold += 10; break;
      case 6: mq135Threshold += 10; break;
      case 7: mq7Threshold += 10; break;
      case 8: turbidityThreshold += 10; break;
      default: break;
    }
    buzzerBeep(100);   // 长按完成提示音
  }
}