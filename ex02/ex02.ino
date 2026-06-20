/*
  作业2 (ex02) - 使用 millis() 控制 LED 以 1Hz 频率稳定闪烁
  不再使用 delay()，而是通过 millis() 获取系统运行时间，
  实现非阻塞的定时闪烁。
*/

const int ledPin = 2;          // ESP32 板载 LED 引脚 (GPIO2)
const long interval = 500;     // 闪烁间隔时间 500ms (1Hz = 1000ms周期, 高低各500ms)
unsigned long previousMillis = 0;  // 上次 LED 状态改变的时间
int ledState = LOW;            // LED 当前状态

void setup() {
  Serial.begin(115200);        // 初始化串口通信
  pinMode(ledPin, OUTPUT);     // 设置 LED 引脚为输出模式
  Serial.println("ex02: millis() 1Hz LED Blink started!");
}

void loop() {
  unsigned long currentMillis = millis();  // 获取当前系统运行时间(ms)

  // 判断是否到达翻转 LED 的时间间隔
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // 记录本次翻转时间

    // 翻转 LED 状态
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    digitalWrite(ledPin, ledState);

    // 串口输出当前 LED 状态
    Serial.print("LED ");
    Serial.println(ledState == HIGH ? "ON" : "OFF");
  }
}
