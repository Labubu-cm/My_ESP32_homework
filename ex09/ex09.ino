/*
  作业9 (ex09) - 实时传感器Web仪表盘
  关联基础：实验4 (触摸引脚)、实验5 (Web服务器)

  功能:
    - ESP32 Web Server 下发HTML页面
    - 提供 /api/touch 接口实时返回触摸传感器数值
    - 网页端使用AJAX不断拉取数值并显示在页面中央
    - 实现类似仪器仪表的数据实时监控面板
*/

#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "RedmiK70Pro";
const char* password = "11111111";

#define TOUCH_PIN 4
#define THRESHOLD 60   // 判定触摸的阈值，需根据实测调整

WebServer server(80);

String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>传感器仪表盘</title>
  <style>
    body { font-family: Arial; text-align: center; margin-top: 50px; }
    #value { font-size: 72px; font-weight: bold; color: #2196F3; }
    #bar { width: 80%; max-width: 400px; height: 30px; background: #e0e0e0;
           border-radius: 15px; margin: 20px auto; overflow: hidden; }
    #fill { height: 100%; background: #4CAF50; border-radius: 15px;
            transition: width 0.3s; }
  </style>
</head>
<body>
  <h1>实时触摸传感器仪表盘</h1>
  <div id="value">0</div>
  <p>触摸数值 (越小表示触摸越近)</p>
  <div id="bar">
    <div id="fill" style="width:0%"></div>
  </div>
  <p id="status">未触摸</p>

  <script>
    function fetchData() {
      fetch('/api/touch')
        .then(response => response.text())
        .then(data => {
          var val = parseInt(data);
          document.getElementById('value').innerText = val;
          // 映射到进度条 (假设范围 0~100)
          var pct = Math.max(0, Math.min(100, 100 - val));
          document.getElementById('fill').style.width = pct + '%';
          document.getElementById('status').innerText =
            val < )rawliteral" + String(THRESHOLD + 5) + R"rawliteral( ? '触摸中' : '未触摸';
        });
    }
    setInterval(fetchData, 500);
  </script>
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

void handleTouch() {
  int touchValue = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(touchValue));
}

void setup() {
  Serial.begin(115200);

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
  server.on("/api/touch", handleTouch);
  server.begin();
}

void loop() {
  server.handleClient();
}
