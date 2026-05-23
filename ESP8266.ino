#include <SoftwareSerial.h>

extern int mq2Value;
extern int lightValue;
extern float temperature;
extern float humidity;

// ===================== 已修改：软串口 11、12 =====================
SoftwareSerial espSerial(11, 12); // RX=11, TX=12
// ==============================================================

// 只发送，不吞数据，不干扰 find()
void espSendCommand(String cmd, int waitMs = 500) {
  espSerial.println(cmd);
  delay(waitMs);
}

void setupESP8266() {
  Serial.println("Initializing ESP8266...");
  
  espSerial.begin(115200);
  delay(3000);

  // 测试AT指令
  espSendCommand("AT", 1000);
  if (espSerial.find("OK")) {
    Serial.println("ESP8266 OK");
  } else {
    Serial.println("ESP8266 not responding");
    return;
  }

  espSendCommand("AT+RST", 3000);
  espSendCommand("AT+CWMODE=2", 2000);
  espSendCommand("AT+CWSAP=\"WIFI\",\"12345678\",1,4", 5000);
  espSendCommand("AT+CIPMUX=1", 500);
  espSendCommand("AT+CIPSERVER=1,8080", 500);

  Serial.println("ESP8266 setup complete. AP WIFI created.");
}

void sendDataViaWiFi() {
  // 纯英文 + 单位 + 一行一个数据（绝不乱码）
  String data = "";
  data += "MQ2: " + String(mq2Value-100) + " ppm\n";
  data += "Light: " + String(lightValue+300) + " lux\n";
  data += "Temp: " + String(temperature) + " C\n";
  data += "Humidity: " + String(humidity+32) + " %RH\n";
  data += "Dust: " + String(dustValue-880) + " ug/m3\n";      // 如果转换后单位是 ug/m³
  // data += "MQ135: " + String(mq135Value) + " ppm\n";     // 如果已转换为 ppm
  // data += "MQ7: " + String(mq7Value) + " ppm\n";
  data += "Turbidity: " + String(turbidityValue-660) + " NTU\n";

  // 一次性发送
  espSendCommand("AT+CIPSEND=0," + String(data.length()), 400);
  
  if (espSerial.find(">")) {
    espSerial.print(data);
    Serial.println("Data sent successfully:");
    Serial.print(data);
  } else {
    Serial.println("Send failed");
  }
}