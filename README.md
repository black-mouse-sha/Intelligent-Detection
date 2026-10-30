<div align="center">

<img src="https://cdn.jsdelivr.net/gh/black-mouse-sha/Intelligent-Detection@main/docs/images/board-top.jpg" width="240" alt="多参数环境监测与自动化控制平台实物">

<h1 align="center">多参数环境监测与自动化控制平台</h1>

<p align="center">
  <b>采到 → 显示 → 判阈值 → 报警 → 上报手机。</b><br>
  一块洞洞板上的 Arduino 环境站：5 类参数在线采集，OLED 本地显示，超标联动继电器，手机连热点就能看数和开关排水。
</p>

<p align="center">
  作者：<a href="https://github.com/black-mouse-sha"><b>LittlePerson</b></a>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/MCU-Arduino%20Nano-00979D?style=flat-square" alt="Arduino Nano">
  <img src="https://img.shields.io/badge/WiFi-ESP--01S%20AT-2f6fed?style=flat-square" alt="ESP-01S">
  <img src="https://img.shields.io/badge/%E4%BC%A0%E6%84%9F%E5%99%A8-5%20%E7%B1%BB%E5%9C%A8%E7%BA%BF-43d19e?style=flat-square" alt="5 类传感器">
  <img src="https://img.shields.io/badge/License-MIT-43d19e?style=flat-square" alt="MIT">
</p>

<p align="center">
  <a href="#长什么样">实物与界面</a> ·
  <a href="#硬件清单">硬件清单</a> ·
  <a href="#接线表">接线表</a> ·
  <a href="#系统组成">系统组成</a> ·
  <a href="#主循环流程">主循环流程</a> ·
  <a href="#快速开始">快速开始</a> ·
  <a href="#已知问题">已知问题</a>
</p>

</div>

---

> [!IMPORTANT]
> **烧录前先把整个文件夹改名为 `esp8266`。**
> Arduino IDE 要求 sketch 文件名与所在文件夹同名，而本工程的主 sketch 是 `ESP8266.ino`、`setup()`/`loop()` 写在 `byesp8266.ino` 里。文件夹名不对会直接编译报错。

## 长什么样

| 实物：洞洞板整机 | 细节：土壤湿度探针与继电器 |
|:---:|:---:|
| <img src="https://cdn.jsdelivr.net/gh/black-mouse-sha/Intelligent-Detection@main/docs/images/board-top.jpg" width="420" alt="整机俯拍"> | <img src="https://cdn.jsdelivr.net/gh/black-mouse-sha/Intelligent-Detection@main/docs/images/board-detail.jpg" width="420" alt="侧面细节"> |
| Nano 居中，右侧 OLED，左上 ESP-01S，下方继电器 | 浊度探头、土壤探针、雨滴板都接在同一块板子上 |

| OLED 本地显示 | 手机端监控页 |
|:---:|:---:|
| <img src="https://cdn.jsdelivr.net/gh/black-mouse-sha/Intelligent-Detection@main/docs/images/oled-display.jpg" width="420" alt="OLED 显示"> | <img src="https://cdn.jsdelivr.net/gh/black-mouse-sha/Intelligent-Detection@main/docs/images/phone-app.jpg" width="210" alt="手机端"> |
| 温湿度、光照、当前调节阈值分屏显示 | 连上热点后实时收数，底部可直接开关排水继电器 |

> 上方两张实拍是同一块板子的不同角度；手机端页面在另一台设备上，本仓库只含下位机侧代码。

## 它能做什么

| 能力 | 说明 |
|---|---|
| 5 类参数在线采集 | 可燃气体（MQ-2）、温湿度（DHT11）、光照（光敏）、粉尘（GP2Y1010AU0F）、浊度 |
| 本地显示 | SSD1306 128×64 OLED，分屏显示实时值与当前正在调节的阈值 |
| 现场调阈值 | 按键短按在 8 个调节项间循环，长按 1 秒按当前项累加，调完立刻生效 |
| 超标联动 | 任一参数越限即点亮 LED、鸣响蜂鸣器、吸合继电器（可外接排风扇、水泵等） |
| 手机远程监控 | ESP-01S 开热点 + TCP 服务，手机连上即可收到 6 行 ASCII 数据并远程开关继电器 |
| 模块化扩展 | 每个传感器 / 执行器一个 `.ino`，新增一路只需追加单文件并在主循环里加两行调用 |

## 硬件清单

| 模块 | 型号 | 数量 | 说明 |
|---|---|:--:|---|
| 主控 | Arduino Nano（ATmega328P） | 1 | 采集、判阈、显示、串口调度 |
| WiFi | ESP-01S | 1 | 只跑 AT 固件，软串口通信，不承担业务逻辑 |
| 温湿度 | DHT11 | 1 | 单总线，读取失败时保留上次值 |
| 可燃气体 | MQ-2 | 1 | 模拟输出，需预热 |
| 光照 | 光敏电阻模块（AO 输出） | 1 | 模拟量 0–1023 |
| 粉尘 | Sharp GP2Y1010AU0F | 1 | 需要红外 LED 驱动脚，按数据手册时序采样 |
| 浊度 | 浊度传感器模块 | 1 | 模拟输出 |
| 显示 | SSD1306 128×64 OLED | 1 | I²C，U8g2 软件 I²C 驱动 |
| 报警 | 有源蜂鸣器 + LED | 各 1 | 高电平触发 |
| 执行 | 继电器模块 SRD-05VDC-SL-C | 1 | 高电平吸合，可带 220V 负载（注意安全） |
| 输入 | 轻触按键 | 1 | `INPUT_PULLUP`，内部上拉 |
| 载体 | 洞洞板 + 杜邦线 | 1 | 手工焊接 |

## 接线表

引脚号对应 Arduino Nano 丝印，全部来自当前源码里的 `#define`。

| 功能 | 宏 | 引脚 | 模式 |
|---|---|---|---|
| MQ-2 可燃气体 | `MQ2_PIN` | A0 | 模拟输入 |
| 光敏 | `LDR_PIN` | A1 | 模拟输入 |
| 粉尘 | `DUST_PIN` / `DUST_LED_PIN` | A2 / D6 | 模拟输入 / 数字输出 |
| 浊度 | `TURBIDITY_PIN` | A5 | 模拟输入 |
| DHT11 数据 | `DHTPIN` | D2 | 单总线 |
| OLED | `OLED.ino` 构造函数 | D10（SCL）/ D9（SDA） | 软件 I²C |
| 按键 | `BUTTON_PIN` | D4 | `INPUT_PULLUP` |
| 蜂鸣器 | `BUZZER_PIN` | D3 | 数字输出 |
| 状态灯 | `LED_PIN` | D5 | 数字输出 |
| 继电器 | `RELAY_PIN` | D7 | 数字输出 |
| ESP-01S | `espSerial(11, 12)` | D11（RX）/ D12（TX） | 软串口 115200 |

> [!NOTE]
> `MQ135.ino`（A3）、`MQ7.ino`（A4）、`RainSensor.ino`（A5）、`SoilMoisture.ino`（A5）四个驱动已经写好，但**没有并入主循环**：Nano 只有 A0–A7，其中 A4/A5 还被 I²C 和浊度占用，雨量与土壤湿度又都写在 A5 上，接不全。详见[已知问题](#已知问题)。

## 系统组成

![系统组成](https://cdn.jsdelivr.net/gh/black-mouse-sha/Intelligent-Detection@main/docs/images/diagrams/architecture.png)

<sub>交互版（可缩放、可切换主题、节点带源码定位）：[docs/diagrams/esp-monitor-architecture.html](https://black-mouse-sha.github.io/Intelligent-Detection/diagrams/esp-monitor-architecture.html)，浏览器直接打开即可。</sub>

一条从传感器到手机的单向链路：传感器阵列 → Nano 主控 → 本地 OLED 与执行器；联网这条支路交给 ESP-01S，它只做 AT 指令的搬运，不参与业务判断。

## 主循环流程

![主循环流程](https://cdn.jsdelivr.net/gh/black-mouse-sha/Intelligent-Detection@main/docs/images/diagrams/loop.png)

<sub>交互版：[docs/diagrams/esp-monitor-loop.html](https://black-mouse-sha.github.io/Intelligent-Detection/diagrams/esp-monitor-loop.html)</sub>

一轮 `loop()` 约 200 ms：读全部在线传感器 → 处理按键 → 刷新 OLED → 逐项比对阈值 → 组装数据帧上报。越限时进入报警分支，打开声光与继电器；正常时关闭。

## 数据上报格式

手机连上热点 `WIFI` 后，用任意 TCP 客户端连 `192.168.4.1:8080`，服务端每轮推送 6 行纯 ASCII：

```text
MQ2: 253 ppm
Light: 385 lux
Temp: 21.00 C
Humidity: 49.00 %RH
Dust: 108 ug/m3
Turbidity: 126 NTU
```

> 数值带固定偏移（`mq2Value-100`、`dustValue-880`、`turbidityValue-660`、`lightValue+300`、`humidity+32`），用于把原始 ADC 读数大致挪到量纲上。**这不是标定曲线**，只适合做相对比较与趋势判断，不能当计量仪表用。

## 快速开始

### 1. 装库

Arduino IDE 的库管理器里装这三个：

- `U8g2`（OLED 驱动）
- `DHT sensor library`（DHT11）
- `SoftwareSerial`（IDE 自带，无需安装）

### 2. 摆好目录

```text
esp8266/                ← 文件夹名必须是这个
├─ ESP8266.ino          ← 主 sketch（承载 setup/loop 的那个目录名）
├─ byesp8266.ino        ← setup() / loop() / checkAlarms()
├─ OLED.ino
├─ DHT11.ino
├─ MQ2.ino
├─ LDR.ino
├─ DustSensor.ino
├─ Turbidity.ino
├─ Button.ino
├─ LED.ino
├─ Buzzer.ino
├─ Relay.ino
├─ ESP8266.ino
├─ MQ135.ino
├─ MQ7.ino
├─ RainSensor.ino
└─ SoilMoisture.ino
```

### 3. 烧录

1. 板卡选 **Arduino Nano**，处理器选 **ATmega328P**（老款选 Old Bootloader）
2. 端口选对应的 COM 口
3. 打开串口监视器，波特率 **115200**
4. 看到 `System Ready` 即启动完成；接着会自动执行 `AT` 握手，成功后打印 `ESP8266 setup complete. AP WIFI created.`

### 4. 连手机

1. 手机 Wi-Fi 连 `WIFI`，密码 `12345678`
2. TCP 客户端连 `192.168.4.1:8080`
3. 每 200 ms 收到一帧数据；发指令可控制继电器（排水开 / 关）

## 已知问题

都是真的，不是没做完。

| 问题 | 现状 | 影响 |
|---|---|---|
| **阈值判据曾写成 `==`** | 已在本次修订改为 `>=`（`checkAlarms()` 内 5 处） | 原版只有数值恰好等于阈值才报警，实际几乎不触发 |
| **模拟口不够用** | 浊度、雨量、土壤湿度都定义在 A5 | 三者只能接一路；OLED 上 `S:` / `R:` 读到 4092 就是这个原因 |
| **MQ135 / MQ7 未启用** | `setupMQ135()`、`readMQ135()` 等在主文件里被注释 | 代码在，数据不进主循环 |
| **换算靠固定偏移** | 见[数据上报格式](#数据上报格式) | 数值只能做相对比较，不是标定后的物理量 |
| **阈值掉电丢失** | 存在 RAM 变量里 | 每次上电回到默认值，未写 EEPROM |
| **热点名与密码硬编码** | `AT+CWSAP="WIFI","12345678",1,4` | 换环境需要改代码重烧，未做配置外置 |
| **手机端页面不在本仓库** | 仅有下位机代码与协议 | 需要自行用任意 TCP 客户端或另写前端 |

## 下一步

- [ ] 上 ADS1115 扩展模拟口，把浊度、雨量、土壤湿度全部接入
- [ ] 关键传感器做多点采样标定，替换固定偏移
- [ ] 阈值与校准参数写入 EEPROM，掉电保留
- [ ] WiFi 从 AP 模式改为 STA 接路由器，改用 MQTT 上报
- [ ] 手机端页面开源，做成可配置的仪表盘

## 工程笔记（给面试官 / 维护者）

- **一个模块一个文件**：每个传感器只有 `setupXxx()` / `readXxx()` 两个入口，全局量用 `extern` 在主文件汇总。加一路传感器 = 新增一个 `.ino` + 主循环加一行，改错的面积很小。
- **粉尘传感器按数据手册时序采样**：`readDust()` 里 280 µs / 40 µs / 9680 µs 三段延时是 GP2Y1010AU0F 要求的红外 LED 与采样窗口，不是随手写的 `delay`。
- **按键做了消抖和长短按分离**：20 ms 消抖，短按切换调节项（1–8 循环），长按 1 s 才累加，避免误触把阈值改乱。
- **OLED 用 U8g2 的 page buffer 模式**：`firstPage()` / `nextPage()` 循环刷屏，128×64 全屏只需 1 KB RAM，Nano 的 2 KB 才够用。
- **失败要能降级**：DHT11 读失败不抛错也不阻塞，保留上一次读数继续跑，整机不会因为一个传感器掉线停摆。
- **串口是主要调试面**：115200 打印每一步状态，现场没有示波器时靠日志定位是哪个模块没起来。

## 目录结构

```text
.
├─ byesp8266.ino          # setup / loop / 报警判定
├─ ESP8266.ino            # 软串口 + AT 指令 + 数据上报
├─ OLED.ino               # U8g2 显示
├─ DHT11.ino  MQ2.ino  LDR.ino
├─ DustSensor.ino  Turbidity.ino
├─ MQ135.ino  MQ7.ino  RainSensor.ino  SoilMoisture.ino   # 驱动已写，未接入
├─ Button.ino  LED.ino  Buzzer.ino  Relay.ino
└─ docs/
   ├─ images/             # 实拍图与界面截图
   └─ diagrams/           # 两张交互式架构图（HTML，浏览器打开）
```

## License

[MIT](LICENSE) © 2026 LittlePerson
