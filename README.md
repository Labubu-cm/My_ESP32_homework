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
  - `sos_video.mp4` — SOS 信号演示视频
- `ex04/` — 作业4：基于触摸传感器的"自锁"开关
  - `ex04.ino` — 使用 touchAttachInterrupt 中断检测触摸，翻转 LED 状态（摸一次亮，再摸灭）
  - `led_on.jpg` / `led_off.jpg` — LED 亮/灭实物图
  - `serial_output.png` — 串口输出截图
- `ex05/` — 作业5：多档位触摸调速呼吸灯
  - `ex05.ino` — 触摸切换三档呼吸速度（慢/中/快），PWM 呼吸 + 中断调速
  - `serial_output.png` — 串口输出截图
- `ex06/` — 作业6：警车双闪灯效（双通道 PWM）
  - `ex06.ino` — 两路 PWM 反相输出，实现红蓝交替渐变闪烁效果
  - `G_on--R_off.jpg` / `R_on--G_off.jpg` — 交替闪实物图
  - `serial_output.png` — 串口输出截图
- `ex07/` — 作业7：Web 网页端无极调光器
  - `ex07.ino` — ESP32 Web 服务器 + HTML 滑动条，通过 fetch 实时调节 LED 亮度
  - `real_result.jpg` — 调光效果实物图
  - `Web.png` / `serial_output.png` — 网页界面和串口截图
- `ex08/` — 作业8：物联网安防报警器模拟
  - `ex08.ino` — Web 布防/撤防 + 触摸触发报警（LED 高频闪烁），撤防前持续报警
  - `unalarmed.jpg` / `warning.jpg` — 正常/报警状态实物图
- `ex09/` — 作业9：实时传感器 Web 仪表盘
  - `ex09.ino` — Web 仪表盘 + AJAX 轮询，实时显示触摸传感器数值及进度条
  - `serial_output.png` — 串口输出截图

## 各实验说明

### Lab02: LED 基本闪烁
- 使用 `delay(1000)` 实现 1Hz 闪烁
- 串口输出 LED ON / LED OFF
- 实物图：`p2_on.jpg` / `p2_off.jpg`，串口输出：`serial_out.png`

### Lab03: LED 呼吸灯
- 使用 PWM (频率 5000Hz, 分辨率 8位)
- 循环 0→255→0 实现呼吸灯效果
- 串口输出呼吸周期完成信息
- 实物图：`pwm_low.jpg` / `pwm_high.jpg`，串口输出：`serial_out.png`

### ex02: millis() 非阻塞 LED 闪烁
- 使用 `millis()` 替代 `delay()` 实现非阻塞定时
- LED 以 500ms 间隔翻转 (1Hz)
- 串口输出 LED 状态、闪烁次数及系统运行时间
- 实物图：`led_on.jpg` / `led_off.jpg`，串口输出：`serial_out.png`

### ex03: millis() SOS 信号
- 使用 `millis()` 状态机实现 SOS Morse 码
- SOS = 短闪(.)+短闪(.)+短闪(.) + 长闪(-)+长闪(-)+长闪(-) + 短闪(.)+短闪(.)+短闪(.)
- 时序：短闪200ms, 长闪600ms, 闪间200ms, 字母间400ms, 循环间2000ms
- 串口输出可视化的 ... --- ... 图案及系统时间
- 演示视频：`sos_video.mp4`

### ex04: 触摸自锁开关
- 使用 `touchAttachInterrupt` 中断方式检测触摸
- 布尔型状态变量 `ledState` 保存 LED 状态
- 在 ISR 中翻转 LED 状态，实现"摸一次亮，再摸一次灭"
- 中断触发阈值可配置（默认60）

### ex05: 多档位触摸调速呼吸灯
- 结合呼吸灯 PWM 代码和触摸中断
- 三档速度循环切换：慢速(15ms/步) → 中速(8ms/步) → 快速(3ms/步)
- ISR 内加入 300ms 防抖逻辑
- 每次触摸切换档位时串口输出当前速度和延时信息

### ex06: 警车双闪灯效
- 两个 LED 连接至不同引脚（GPIO4 / GPIO16），初始化两个独立 PWM 通道
- 两灯亮度呈"反相"关系：A 占空比 0→255 时，B 占空比 255→0
- 实现平滑交替渐变闪烁效果
- 串口输出每个双闪周期完成信息

### ex07: Web 网页端无极调光器
- 网页中加入滑动条 (`<input type="range">`) 控制亮度
- JavaScript 监听滑动条变动，通过 `fetch` 将数值发送给 ESP32
- ESP32 解析 URL 参数并赋值给 PWM 通道
- LED 亮度 0~255 级无极调节

### ex08: 物联网安防报警器
- Web 端提供"布防(Arm)"和"撤防(Disarm)"按钮
- 布防状态下触摸传感器触发报警 → LED 高频闪烁（100ms 间隔）
- 报警后手松开仍持续报警，只有网页点击"撤防"方可停止
- 页面实时显示系统状态和报警状态

### ex09: 实时传感器 Web 仪表盘
- ESP32 Web Server 提供 `/api/touch` 接口实时返回触摸传感器数值
- 网页端使用 AJAX (500ms 轮询) 拉取数值，显示在仪表盘中央
- 触摸数值映射为进度条宽度、触摸/未触摸状态提示
- 类似仪器仪表的实时数据监控面板
