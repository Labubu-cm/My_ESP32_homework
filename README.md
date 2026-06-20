# My_ESP32_homework
用来上传我的程序设计实践作业

## 项目结构

- `ex01/` — 实验1：Hello World 程序
  - `hello.c` — C 语言 Hello World 程序，支持键盘输入姓名并显示问候
- `01_lab/` — Lab01：ESP32 LED Blink 实验
  - `01_lab.ino` — ESP32 程序，GPIO2 控制 LED 每秒闪烁，串口输出 "Hello ESP32!"
  - `p2_off.jpg` — LED 熄灭状态实物图
  - `p2_on.jpg` — LED 点亮状态实物图
  - `serial_output.png` — 串口监视器输出截图
- `lab02/` — Lab02：ESP32 LED Blink (delay方式)
  - `lab02.ino` — 使用 delay() 实现 LED 1Hz 闪烁，串口输出 LED ON/OFF 状态
- `lab03/` — Lab03：ESP32 LED 呼吸灯 (PWM)
  - `lab03.ino` — 使用 ledcAttach/ledcWrite 实现 LED 呼吸效果 (PWM 8位, 5kHz)
- `ex02/` — 作业2：使用 millis() 控制 LED 以 1Hz 稳定闪烁
  - `ex02.ino` — 非阻塞实现，使用 millis() 替代 delay()，带闪烁计数和系统时间输出
- `ex03/` — 作业3：使用 millis() 产生 SOS 闪烁信号 (... --- ...)
  - `ex03.ino` — 状态机实现 SOS 模式 (3短3长3短)，带文本可视化输出

## 各实验说明

### Lab02: LED 基本闪烁
- 使用 `delay(1000)` 实现 1Hz 闪烁
- 串口输出 LED ON / LED OFF

### Lab03: LED 呼吸灯
- 使用 PWM (频率 5000Hz, 分辨率 8位)
- 循环 0→255→0 实现呼吸灯效果
- 串口输出呼吸周期完成信息

### ex02: millis() 非阻塞 LED 闪烁
- 使用 `millis()` 替代 `delay()` 实现非阻塞定时
- LED 以 500ms 间隔翻转 (1Hz)
- 串口输出 LED 状态、闪烁次数及系统运行时间

### ex03: millis() SOS 信号
- 使用 `millis()` 状态机实现 SOS Morse 码
- SOS = 短闪(.)+短闪(.)+短闪(.) + 长闪(-)+长闪(-)+长闪(-) + 短闪(.)+短闪(.)+短闪(.)
- 时序：短闪200ms, 长闪600ms, 闪间200ms, 字母间400ms, 循环间2000ms
- 串口输出可视化的 ... --- ... 图案及系统时间
