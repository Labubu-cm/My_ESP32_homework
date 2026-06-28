/*
  作业7 (ex07) - Web网页端无极调光器
  关联基础：实验3 (PWM呼吸灯)、实验5 (Web服务器)

  功能:
    - 网页中加入滑动条 (<input type="range" min="0" max="255">)
    - JavaScript 监听滑动条变动，通过 fetch 将数值发送给ESP32
    - ESP32解析URL中的数值并赋值给PWM通道
*/

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "RedmiK70Pro";
const char* password = "11111111";
const int LED_PIN = 2;

WebServer server(80);

String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>无极调光器</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    input[type=range] { width: 80%; max-width: 400px; margin: 20px; }
    #dutyVal { font-size: 24px; font-weight: bold; }
  </style>
</head>
<body>
  <h1>ESP32 无极调光器</h1>
  <p>拖动滑动条调节LED亮度</p>
  <input type="range" min="0" max="255" value="0" id="dutySlider"
    oninput="updateDuty(this.value)">
  <p>占空比: <span id="dutyVal">0</span></p>

  <script>
    function updateDuty(val) {
      document.getElementById('dutyVal').innerText = val;
      fetch('/set?duty=' + val);
    }
  </script>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handleSet() {
  if (server.hasArg("duty")) {
    int duty = server.arg("duty").toInt();
    duty = constrain(duty, 0, 255);
    ledcWrite(LED_PIN, duty);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing duty");
  }
}

void setup() {
  Serial.begin(115200);

  ledcAttach(LED_PIN, 5000, 8);

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
  server.on("/set", handleSet);
  server.begin();
}

void loop() {
  server.handleClient();
}
