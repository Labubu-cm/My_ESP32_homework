/*
  作业6 (ex06) - 警车双闪灯效 (双通道PWM)
  关联基础：实验3 (PWM呼吸灯)

  功能:
    - 两个LED连接至不同引脚，初始化两个独立PWM通道
    - 两个灯亮度呈"反相"关系:
      灯A占空比 0→255 时，灯B占空比 255→0
    - 实现平滑交替渐变闪烁效果

  硬件连接:
    LED_A → GPIO4 
    LED_B → GPIO16 (外接LED)
*/

// 定义LED引脚
const int ledPinA = 4;
const int ledPinB = 16;

// 设置PWM属性
const int freq = 5000;
const int resolution = 8;

void setup() {
  Serial.begin(115200);

  ledcAttach(ledPinA, freq, resolution);
  ledcAttach(ledPinB, freq, resolution);

  // 初始状态: A最亮, B最暗
  ledcWrite(ledPinA, 255);
  ledcWrite(ledPinB, 0);
}

void loop() {
  // 相位1: A逐渐变亮 (0→255), B逐渐变暗 (255→0)
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
    ledcWrite(ledPinA, dutyCycle);
    ledcWrite(ledPinB, 255 - dutyCycle);
    delay(8);
  }

  // 相位2: A逐渐变暗 (255→0), B逐渐变亮 (0→255)
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
    ledcWrite(ledPinA, dutyCycle);
    ledcWrite(ledPinB, 255 - dutyCycle);
    delay(8);
  }

  Serial.println("Double flash cycle completed");
}
