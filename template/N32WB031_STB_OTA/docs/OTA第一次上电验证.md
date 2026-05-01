# OTA 第一次上电验证

## 1. 先说结论

如果你只是：

- 烧了 `masterboot`
- 烧了 `image_update`
- 烧了 `app_ota`

但**没有烧 `bootsetting.bin`**，那么非常容易出现这种现象：

- 板子上电没明显反应
- LED 不亮
- 串口没日志
- 手机也搜不到 OTA 蓝牙

原因不是工程坏了，而是：

**`masterboot` 上电后要先看 `bootsetting`，决定到底该跳到 `APP1`、`APP2` 还是 `ImageUpdate`。如果这个配置区没准备好，启动链路就不完整。**

## 2. 这次最重要的文件

第一次最小验证，最重要的其实不是三个工程本身，而是下面这几类镜像：

- `MasterBoot.bin`
- `bootsetting.bin`
- `APP1.bin`
- `APP2.bin`
- `ImageUpdate.bin`

你当前仓库里，SDK 已经自带了一份现成的参考镜像目录：

[utilities/dfu/Image](/d:/n32wb031/sdk/N32WB03x_SDK_V2.0.0/N32WB03x_SDK_V2.0.0/utilities/dfu/Image)

里面我已经确认存在：

- `MasterBoot.bin`
- `bootsetting.bin`
- `APP1.bin`
- `APP2.bin`
- `ImageUpdate.bin`

所以第一次上电验证，最稳的做法不是立刻用你自己新编译出来的镜像，而是先用这一套**官方预置好的参考镜像**把启动链路跑通。

## 3. 地址关系

这套 OTA 的核心地址关系如下：

| 区域 | 地址 | 说明 |
| --- | --- | --- |
| `MASTERBOOT` | `0x01000000` | 启动入口 |
| `BOOTSETTING` | `0x01002000` | 启动配置区 |
| `APP1` | `0x01004000` | 应用槽位 1 |
| `APP2` | `0x01020000` | 应用槽位 2 |
| `IMAGE_UPDATE` | `0x0103C000` | 升级执行层 |

## 4. 第一次最小验证，推荐烧录顺序

最推荐的验证方式是：

1. 烧录 `MasterBoot.bin` 到 `0x01000000`
2. 烧录 `bootsetting.bin` 到 `0x01002000`
3. 烧录 `APP1.bin` 到 `0x01004000`
4. 烧录 `ImageUpdate.bin` 到 `0x0103C000`

`APP2.bin` 第一次不是必须的。  
先把 `APP1` 跑起来，证明整个 OTA 静态链路是对的，再说后面的双 Bank 升级。

## 5. 为什么第一次不建议直接用自己编译的 APP1

因为你自己编译出来的 `APP1.axf / .hex` 虽然代码能跑，但：

- `bootsetting.bin` 里的 CRC
- 激活标志
- 版本信息

必须和实际烧进去的镜像一致。

如果你现在还没自己重新生成一份匹配的 `bootsetting.bin`，那直接拿官方自带的 `bootsetting.bin` 去配你自己新编译的镜像，有可能 CRC 不一致。

所以第一次最稳的策略是：

**先用 SDK 自带的成套镜像验证启动链路。**

等链路通了，再进入“用自己编译产物重建 bootsetting”的阶段。

## 6. 烧进去以后，理想现象是什么

如果第一次最小验证成功，上电后应当出现下面这些现象：

1. 板子成功启动，不死机
2. `masterboot` 完成启动判断
3. 最终跳转进入 `APP1`
4. `APP1` 的 LED 启动节奏能看到
5. 串口可能打印类似：

   `application 1 start new ...`

6. 手机能搜到 OTA 应用的蓝牙广播

也就是说：

**你看到的最终现象，应该更像“APP1 正常跑起来了”，而不是“停在 bootloader 里”。**

## 7. 如果还是什么都没有

如果你按上面的顺序烧完：

- `MasterBoot.bin`
- `bootsetting.bin`
- `APP1.bin`
- `ImageUpdate.bin`

上电后仍然：

- LED 没反应
- 串口没反应
- 手机也搜不到

那就优先怀疑下面几项：

1. 烧录地址不对
2. 实际烧进去的不是 SDK 自带镜像，而是你自己编译出来的镜像
3. `bootsetting.bin` 和实际镜像不匹配
4. 烧录工具没有真的写进去

## 8. 你现在最稳的动作

现在最推荐你这样做：

1. 不用自己刚编译出来的产物
2. 直接用 SDK 自带 `utilities/dfu/Image` 里的参考镜像
3. 按我上面的顺序烧进去
4. 上电观察现象

只要这一步通了，就说明：

- OTA 地址布局对
- bootsetting 机制对
- masterboot 跳转对
- APP1 运行链路对

## 9. 通了以后再做什么

第一次最小验证通了以后，下一步才进入更深入的部分：

1. 用你自己编译出来的 `APP1 / APP2 / ImageUpdate`
2. 重新生成匹配的 `bootsetting.bin`
3. 再做双 Bank OTA 升级验证

也就是说，第一次目标不是“完整 OTA”，而是：

**先把启动链路跑通。**
