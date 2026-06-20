/*
  实验2 (lab02) - 基础IO实验: LED闪烁控制
  掌握 ESP32 GPIO 输出控制基本方法

  内容:
    1. 基础实验: 使用 delay() 实现 LED 1Hz 闪烁
    2. 进阶实验: SOS 求救信号 (delay() 版本)
*/

const int ledPin = 2;

// ============================================================
// 基础实验: LED 1Hz 闪烁
// ============================================================
void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // ---- 基础闪烁 ----
  digitalWrite(ledPin, HIGH);
  Serial.println("LED ON");
  delay(1000);

  digitalWrite(ledPin, LOW);
  Serial.println("LED OFF");
  delay(1000);

  // ============================================================
  // 进阶实验: SOS 求救信号 (取消注释下方代码即可启用)
  // 摩尔斯电码: 三短(S) + 三长(O) + 三短(S)
  // ============================================================
  // sosSignal();
}

// SOS 求救信号函数
void sosSignal() {
  Serial.println("SOS Signal Start!");

  // S: 三短闪 (.)
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  delay(500);  // 字母间隔

  // O: 三长闪 (-)
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(600);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  delay(500);  // 字母间隔

  // S: 三短闪 (.)
  for (int i = 0; i < 3; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }

  // 单词间隔
  delay(2000);
  Serial.println("SOS Signal End");
}
