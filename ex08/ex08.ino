/*
  作业8 (ex08) - 物联网安防报警器模拟实验
  关联基础：实验2 (基础IO)、实验4 (触摸引脚)、实验5 (Web服务器)

  功能:
    - 网页端提供"布防(Arm)"和"撤防(Disarm)"两个按钮
    - 全局变量记录系统状态 (armed)
    - 布防状态下触摸引脚 → LED狂闪（报警），手松开也持续报警
    - 只有网页点击"撤防"才停止报警
*/

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "RedmiK70Pro";
const char* password = "11111111";

#define LED_PIN 2
#define TOUCH_PIN 4
#define THRESHOLD 60

WebServer server(80);

bool armed = false;        // 布防状态
bool warning = false;        // 报警状态 (不要用 alarm，和系统函数冲突)

// 中断服务函数 (ISR)
void gotTouch() {
  if (armed) {
    warning = true;   // 布防状态下触摸 → 触发报警
  }
}

String makePage() {
  String armStatus = armed ? "已布防" : "已撤防";
  String alarmStatus = warning ? "!! 报警中 !!" : "正常";
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>安防报警器</title>
</head>
<body style="font-family:Arial; text-align:center; margin-top:50px;">
  <h1>物联网安防报警器</h1>
  <p>系统状态: <b>)rawliteral" + armStatus + R"rawliteral(</b></p>
  <p>报警状态: <b>)rawliteral" + alarmStatus + R"rawliteral(</b></p>
  <a href="/arm"><button style="padding:10px 20px; margin:5px;">布防 (Arm)</button></a>
  <a href="/disarm"><button style="padding:10px 20px; margin:5px;">撤防 (Disarm)</button></a>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handleArm() {
  armed = true;
  warning = false;   // 布防时清除报警
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleDisarm() {
  armed = false;
  warning = false;   // 撤防时清除报警
  digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // 绑定触摸中断
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);

  // 连接 WiFi
  WiFi.begin(ssid, password);
  Serial.print("连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n连接成功");
  Serial.print("访问地址: http://");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void loop() {
  server.handleClient();

  // 报警状态下 LED 高频闪烁
  if (warning) {
    static unsigned long lastFlash = 0;
    static bool flashState = false;
    if (millis() - lastFlash > 100) {
      flashState = !flashState;
      digitalWrite(LED_PIN, flashState);
      lastFlash = millis();
    }
  }
}
