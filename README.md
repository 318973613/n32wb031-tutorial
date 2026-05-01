# N32WB031 BLE SoC 教程

国民技术 N32WB031 蓝牙 5.1 SoC 开发教程，从开箱到 BLE 透传、OLED 显示、HID 蓝牙遥控器、OTA 空中升级，提供完整的代码和文档。

## 芯片简介

| 参数 | 规格 |
|------|------|
| 内核 | ARM Cortex-M0, 64MHz |
| SRAM | 64KB |
| Flash | 256KB / 512KB |
| 蓝牙 | BLE 5.1, 支持长距离、多连接、Mesh |
| 发射功率 | 最高 +6dBm |
| 低功耗 | Sleep 1.4uA, 掉电 130nA |

## 教程目录

| 章节 | 内容 | 涉及工程 |
|------|------|----------|
| 第一章 | 开箱与开发板介绍 | - |
| 第二章 | 开发环境搭建 | - |
| 第三章 | LED 点灯 | `Blank_LED` |
| 第四章 | 按键与串口 | `Key_USART` |
| 第五章 | BLE 透传 (RDTSS) | `BLE_RDTSS` |
| 第六章 | OLED 显示 | `BLE_RDTSS_OLED` |
| 第七章 | HID 蓝牙拍照遥控器 | `HID_SWIPE_OLED` |
| 第八章 | BLE OTA 空中升级 | `OTA` |

完整文档见 [`doc/N32WB031 测评合集.md`](doc/N32WB031%20测评合集.md)。

## 工程说明

```
template/
├── N32WB031_STB_Blank_LED/          # LED 闪烁
├── N32WB031_STB_Key_USART/          # 按键检测 + 串口通信
├── N32WB031_STB_BLE_RDTSS/          # BLE 透传服务
├── N32WB031_STB_BLE_RDTSS_OLED/     # BLE 透传 + OLED 显示
├── N32WB031_STB_HID_SWIPE_OLED/     # HID 蓝牙拍照遥控器
└── N32WB031_STB_OTA/                # 空中升级
```

所有工程基于 Keil MDK-ARM，需要单独下载 N32WB031 SDK（国民技术官网）。

## 快速开始

1. 安装 Keil MDK-ARM 和 N32WB031 器件包
2. 下载 [N32WB031 SDK](https://www.nationstech.com/)，解压到项目根目录
3. 用 Keil 打开任意工程的 `.uvprojx` 文件
4. 编译、下载、观察效果

## 硬件接线

以 HID 拍照遥控器工程为例：

| 功能 | 引脚 | 说明 |
|------|------|------|
| LED1 | PB0 | 运行指示 |
| LED2 | PA6 | 连接状态 |
| KEY1 | PB1 | 发送 VOL+ |
| KEY2 | PB2 | 发送 VOL- |
| OLED SCL | PB8 | 软件 I2C |
| OLED SDA | PB9 | 软件 I2C |

## License

代码基于国民技术官方 SDK 示例，仅供学习交流。
