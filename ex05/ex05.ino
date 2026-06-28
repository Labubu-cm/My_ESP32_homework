/*
  作业5 (ex05) - 多档位触摸调速呼吸灯
  关联基础：实验3 (PWM呼吸灯)、实验4 (触摸引脚)

  功能:
    - 结合呼吸灯的PWM代码和触摸中断
    - 定义速度档位变量 (speedLevel: 1/2/3)
    - 每次触发触摸引脚，档位循环切换 (1 → 2 → 3 → 1 ...)
    - 在PWM循环中，根据档位改变 delay() 时长
*/

#define TOUCH_PIN 4
#define LED_PIN 2
#define THRESHOLD 70   // 需要根据实际测试修改此阈值

// 设置PWM属性
const int freq = 5000;
const int resolution = 8;

volatile int speedLevel = 1;   // 当前速度档位 (1=慢, 2=中, 3=快)

// 中断服务函数 (ISR) — 切换档位 + 打印变化信息
// 注意: ISR 中用 volatile 变量 + 延时防抖
void IRAM_ATTR gotTouch() {
  static unsigned long lastTouchMs = 0;  // 防抖计时
  unsigned long now = millis();

  // 如果距离上次触发不到 300ms，认为是抖动，忽略
  if (now - lastTouchMs < 300) return;
  lastTouchMs = now;

  // 循环切换档位: 1 → 2 → 3 → 1 ...
  speedLevel++;
  if (speedLevel > 3) {
    speedLevel = 1;
  }

  Serial.print("Breathing cycle completed  |  Speed: ");
  Serial.print(speedLevel);
  Serial.print("  |  Delay: ");
  Serial.print(getSpeedDelay());
  Serial.println(" ms/step");
}

// 根据当前档位返回呼吸灯的步进延时
int getSpeedDelay() {
  if (speedLevel == 1) {
    return 15;   // 慢速呼吸
  } else if (speedLevel == 2) {
    return 8;    // 中速呼吸
  } else {
    return 3;    // 快速呼吸
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  ledcAttach(LED_PIN, freq, resolution);

  // 绑定触摸中断
  touchAttachInterrupt(TOUCH_PIN, gotTouch, THRESHOLD);
}

void loop() {
  int stepDelay = getSpeedDelay();

  // 逐渐变亮
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(LED_PIN, dutyCycle);
    delay(stepDelay);
  }

  // 逐渐变暗
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(LED_PIN, dutyCycle);
    delay(stepDelay);
  }
}
