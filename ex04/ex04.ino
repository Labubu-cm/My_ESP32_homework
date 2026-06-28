/*
  作业4 (ex04) - 基于触摸传感器的"自锁"开关
  关联基础：实验2 (基础IO)、实验4 (触摸引脚)

  功能:
    - 使用 touchAttachInterrupt 中断方式检测触摸
    - 引入布尔型状态变量 (ledState) 保存LED状态
    - 在 ISR 中翻转LED状态，实现"摸一次亮，再摸一次灭"

  预期结果:
    摸一下触摸引脚 → LED亮起并保持长亮
    松开手再摸一下 → LED熄灭
*/

#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 60   // 需要根据实际测试修改此阈值

bool ledState = false;

// 中断服务函数 (ISR)
void gotTouch() {


  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);

  int touchValue = touchRead(TOUCH_PIN);
  Serial.print("Touch Value: ");
  Serial.print(touchValue);
  Serial.print("  |  LED: ");
  Serial.println(ledState ? "ON" : "OFF");
}


void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // 绑定中断函数
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);
}

void loop() {

  delay(100);

}
