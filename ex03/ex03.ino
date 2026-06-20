/*
  作业3 (ex03) - 使用 millis() 控制 LED 产生 SOS 闪烁信号
  SOS 信号：··· --- ··· （三短、三长、三短）
  使用 millis() 实现非阻塞定时，不使用 delay()

  时序定义:
    - 短闪 (.)  : 200ms 亮, 200ms 灭
    - 长闪 (-)  : 600ms 亮, 200ms 灭
    - 字符间停顿: 400ms
    - SOS 循环间: 2000ms 长停顿
*/

const int ledPin = 2;

// SOS 时序参数 (毫秒)
const int shortFlash = 200;    // 短闪点亮时间
const int longFlash  = 600;    // 长闪点亮时间
const int flashGap   = 200;    // 闪灭间隔
const int letterGap  = 400;    // 字符间停顿
const int cyclePause = 2000;   // SOS 循环间长停顿

// 状态定义
enum State {
  FLASH_ON,       // LED 点亮中
  FLASH_OFF,      // LED 熄灭 (闪间停顿)
  LETTER_GAP,     // 字母间停顿
  CYCLE_PAUSE,    // 循环间长停顿
  IDLE            // 初始空闲
};

State currentState = IDLE;
unsigned long previousMillis = 0;
int flashIndex = 0;     // 当前闪烁在序列中的索引
bool flashIsLong = false;  // 当前闪烁是否为长闪

// SOS 闪光序列: true = 长闪, false = 短闪
// S = 短,短,短 | O = 长,长,长 | S = 短,短,短
const bool sosPattern[] = {
  false, false, false,   // S
  true,  true,  true,    // O
  false, false, false    // S
};
const int patternLength = sizeof(sosPattern) / sizeof(sosPattern[0]);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println("====================================");
  Serial.println("ex03: SOS Signal with millis()");
  Serial.println("Pattern: ... --- ... (SOS)");
  Serial.println("Short flash: 200ms | Long flash: 600ms");
  Serial.println("Using millis() for non-blocking timing");
  Serial.println("====================================");

  // 开始第一个 SOS 循环
  currentState = FLASH_ON;
  previousMillis = millis();
  flashIndex = 0;
  flashIsLong = sosPattern[flashIndex];
  digitalWrite(ledPin, HIGH);
  Serial.println("SOS cycle started...");
}

void loop() {
  unsigned long currentMillis = millis();

  switch (currentState) {

    case FLASH_ON: {
      // 等待点亮时间结束
      unsigned long duration = flashIsLong ? longFlash : shortFlash;
      if (currentMillis - previousMillis >= duration) {
        // 熄灭 LED，进入闪间停顿
        digitalWrite(ledPin, LOW);
        previousMillis = currentMillis;
        currentState = FLASH_OFF;

        // 串口输出
        Serial.print(flashIsLong ? "LONG" : "SHORT");
        Serial.print(" flash #");
        Serial.print(flashIndex + 1);
        Serial.print("/");
        Serial.print(patternLength);
        Serial.println(" OFF");
      }
      break;
    }

    case FLASH_OFF: {
      // 闪间停顿 (200ms)
      if (currentMillis - previousMillis >= flashGap) {
        flashIndex++;

        // 判断是否已完成整个模式
        if (flashIndex >= patternLength) {
          // 进入循环间长停顿
          previousMillis = currentMillis;
          currentState = CYCLE_PAUSE;
          Serial.println("--- SOS cycle complete ---");
        } else {
          // 判断是否进入字母间停顿 (每3个闪之后)
          if (flashIndex % 3 == 0) {
            previousMillis = currentMillis;
            currentState = LETTER_GAP;
          } else {
            // 继续下一个闪
            flashIsLong = sosPattern[flashIndex];
            digitalWrite(ledPin, HIGH);
            previousMillis = currentMillis;
            currentState = FLASH_ON;

            Serial.print(flashIsLong ? "LONG" : "SHORT");
            Serial.print(" flash #");
            Serial.print(flashIndex + 1);
            Serial.println(" ON");
          }
        }
      }
      break;
    }

    case LETTER_GAP: {
      // 字母间停顿 (400ms)
      if (currentMillis - previousMillis >= letterGap) {
        flashIsLong = sosPattern[flashIndex];
        digitalWrite(ledPin, HIGH);
        previousMillis = currentMillis;
        currentState = FLASH_ON;

        Serial.print("--- letter gap ---  ");
        Serial.print(flashIsLong ? "LONG" : "SHORT");
        Serial.print(" flash #");
        Serial.print(flashIndex + 1);
        Serial.println(" ON");
      }
      break;
    }

    case CYCLE_PAUSE: {
      // 循环间长停顿 (2000ms)
      if (currentMillis - previousMillis >= cyclePause) {
        // 重新开始新循环
        flashIndex = 0;
        flashIsLong = sosPattern[0];
        digitalWrite(ledPin, HIGH);
        previousMillis = currentMillis;
        currentState = FLASH_ON;

        Serial.println("--- New SOS cycle ---");
        Serial.print(flashIsLong ? "LONG" : "SHORT");
        Serial.print(" flash #1/");
        Serial.print(patternLength);
        Serial.println(" ON");
      }
      break;
    }

    default:
      break;
  }
}
