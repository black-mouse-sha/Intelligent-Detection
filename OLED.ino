// OLED.ino
#include <U8g2lib.h>

// 构造函数：U8G2_SSD1306_128X64_NONAME_1_SW_I2C(rotation, clock, data, reset)
U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/ 10, /* data=*/ 9, /* reset=*/ U8X8_PIN_NONE);

// 外部变量声明（实际定义在其他模块中）
extern int mq2Value, dustValue;
extern float temperature, humidity;
extern int mq135Value, mq7Value;
extern int lightValue, turbidityValue;
extern int adjustMode;
extern int mq2Threshold, lightThreshold, tempThreshold, humThreshold;
extern int dustThreshold, mq135Threshold, mq7Threshold, turbidityThreshold;

void setupOLED() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB08_tr);
  u8g2.setColorIndex(1);
}

// 显示单条信息（开机画面用）
void displayMessage(const char* msg, int delayMs) {
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 12);
    u8g2.print(msg);
  } while (u8g2.nextPage());
  delay(delayMs);
}

// 刷新所有传感器数据（U8g2 版本，5行显示所有数据）
void updateDisplay() {
  u8g2.firstPage();
  do {
    // 第一行：MQ2 和 粉尘
    u8g2.setCursor(0, 10);
    u8g2.print("MQ2: "); u8g2.print(mq2Value -100);
    u8g2.print(" ppm D:"); u8g2.print(dustValue-880 );
     u8g2.println("ug/m3");
    // 第二行：温度和湿度
    u8g2.setCursor(0, 22);
    u8g2.print("T:"); u8g2.print(temperature, 1);
    u8g2.print("   C  H: "); u8g2.print(humidity + 32, 1);
    u8g2.println("   %");
    
    // // 第三行：MQ135 和 MQ7
    // u8g2.setCursor(0, 34);
    // u8g2.print("M135:"); u8g2.print(mq135Value);
    // u8g2.print("  M7:"); u8g2.println(mq7Value);
    
    // 第四行：光敏和浊度
    u8g2.setCursor(0, 34);
    u8g2.print("LDR:"); u8g2.print(lightValue+300);
    u8g2.println(" lux\n");
     u8g2.setCursor(0, 46);
    u8g2.print("Turb:  "); u8g2.print(turbidityValue-660);
    u8g2.println("   NTU\n");
    // 第五行：当前调节模式及其阈值
    u8g2.setCursor(0, 58);
    u8g2.print("Th:");
    switch(adjustMode) {
      case 1: u8g2.print("MQ2 "); u8g2.print(mq2Threshold); break;
      case 2: u8g2.print("LDR "); u8g2.print(lightThreshold); break;
      case 3: u8g2.print("Temp "); u8g2.print(tempThreshold); u8g2.print("C"); break;
      case 4: u8g2.print("Hum "); u8g2.print(humThreshold); u8g2.print("%"); break;
      case 5: u8g2.print("Dust "); u8g2.print(dustThreshold); break;
      // case 6: u8g2.print("M135 "); u8g2.print(mq135Threshold); break;
      // case 7: u8g2.print("M7 "); u8g2.print(mq7Threshold); break;
      case 6: u8g2.print("Turb "); u8g2.print(turbidityThreshold); break;
      default: u8g2.print("None");
    }
  } while (u8g2.nextPage());
}