# N32WB031 OTA 蓝牙升级教程

## 1. 这篇文章要解决什么问题

这篇文章要解决的不是“如何点亮一个灯”，而是一个更接近真实产品交付的问题：

**如何让 N32WB031 具备蓝牙 OTA 升级能力。**

很多人第一次接触 OTA，会误以为它只是一个普通应用工程。  
实际上，OTA 更像是一整套“启动链路 + 应用分区 + 升级执行层 + 配置区”的组合。

所以这篇文章的重点有两个：

1. 搞清楚这套 OTA 工程到底是怎么分工的
2. 按正确顺序把它真正跑起来

---

## 2. 先说结论：OTA 不是一个工程

这套 OTA 不是“打开一个工程编译下载就完事”，而是由三类工程和一个公共组件共同组成：

### 2.1 三件套工程

- `masterboot`
  - 负责上电后最先启动
  - 决定到底跳到哪一份应用
  - 必要时进入升级流程

- `app_ota`
  - 正常运行的蓝牙 OTA 主应用
  - 手机搜索到的升级设备，通常就是它
  - 负责和手机交互、接收升级数据

- `image_update`
  - 升级执行层
  - 真正完成镜像更新动作

### 2.2 公共组件

- `common`
  - 存放公共 DFU 文件
  - 比如 CRC、延时、LED、串口升级辅助

所以，OTA 的正确理解应该是：

> `masterboot` 管启动，`app_ota` 管日常运行和接收升级，`image_update` 管升级过程本身。

---

## 3. 当前整理后的工程位置

我已经把这套 OTA 工程整理到这里：

- [template/N32WB031_STB_OTA](/d:/n32wb031/template/N32WB031_STB_OTA)
- [README.txt](/d:/n32wb031/template/N32WB031_STB_OTA/README.txt)

其中主要工程文件是：

- [N32WB031_STB_OTA_MASTERBOOT.uvprojx](/d:/n32wb031/template/N32WB031_STB_OTA/masterboot/MDK-ARM/N32WB031_STB_OTA_MASTERBOOT.uvprojx)
- [N32WB031_STB_OTA_APP.uvprojx](/d:/n32wb031/template/N32WB031_STB_OTA/app_ota/MDK-ARM/N32WB031_STB_OTA_APP.uvprojx)
- [N32WB031_STB_OTA_IMAGE_UPDATE.uvprojx](/d:/n32wb031/template/N32WB031_STB_OTA/image_update/MDK-ARM/N32WB031_STB_OTA_IMAGE_UPDATE.uvprojx)

`[截图位 1：OTA 工程总目录结构]`

---

## 4. OTA 分区地址关系

根据这套工程的链接地址和 `ns_dfu_boot.h` 的定义，当前分区关系如下：

| 区域 | 起始地址 | 大小 | 作用 |
| --- | --- | --- | --- |
| `MASTERBOOT` | `0x01000000` | `0x00002000` | 启动入口 |
| `BOOTSETTING` | `0x01002000` | `0x00001000` | 启动配置区 |
| `APP_DATA` | `0x01003000` | `0x00001000` | 应用数据区 |
| `APP1` | `0x01004000` | `0x0001C000` | 应用槽位 1 |
| `APP2` | `0x01020000` | `0x0001C000` | 应用槽位 2 |
| `IMAGE_UPDATE` | `0x0103C000` | `0x00004000` | 升级执行层 |

这个布局非常关键，因为后面：

- 烧录地址
- bootsetting 内容
- OTA 升级包的目标区域

都依赖这张表。

`[截图位 2：分区地址表或内存布局图]`

---

## 5. masterboot 到底做什么

`masterboot` 是整个 OTA 链路里最底层的一环。

它上电后主要做三件事：

1. 读取 `bootsetting`
2. 判断 `APP1 / APP2 / ImageUpdate` 哪个被激活
3. 检查对应镜像 CRC 是否正确，能跳就跳

也就是说，`masterboot` 并不会“傻傻地永远跳 APP1”。  
它是根据配置区里的激活标志和校验结果动态决定启动目标的。

### 5.1 关键代码

下面这段逻辑最值得看：

```c
if(ns_bootsetting.app1.activation == NS_BOOTSETTING_ACTIVATION_YES
   && ns_bootsetting.app1.start_address == NS_APP1_START_ADDRESS)
{
    if(ns_bootsetting.app1.crc == dfu_crc32(...))
    {
        jump_address = ns_bootsetting.app1.start_address;
    }
}
else if(ns_bootsetting.app2.activation == NS_BOOTSETTING_ACTIVATION_YES
        && ns_bootsetting.app2.start_address == NS_APP2_START_ADDRESS)
{
    ...
}
else if(ns_bootsetting.ImageUpdate.activation == NS_BOOTSETTING_ACTIVATION_YES
        && ns_bootsetting.ImageUpdate.start_address == NS_IMAGE_UPDATE_START_ADDRESS)
{
    ...
}
```

这段代码说明：

- 激活标志决定“优先启动谁”
- CRC 决定“这个镜像能不能信”

如果你理解了这一点，就理解了 OTA 启动链路的一半。

---

## 6. bootsetting 是什么，为什么这么重要

如果说 `masterboot` 是“执行者”，那 `bootsetting` 就是“指挥单”。

它保存了：

- `APP1` 的起始地址、版本、大小、CRC、激活状态
- `APP2` 的起始地址、版本、大小、CRC、激活状态
- `ImageUpdate` 的起始地址、版本、大小、CRC、激活状态
- 是否强制进入升级模式

### 6.1 关键结构体

```c
typedef struct
{
    uint32_t start_address;
    uint32_t size;
    uint32_t crc;
    uint32_t version;
    uint32_t activation;
    uint32_t reserve[5];
} NS_Bank_t;

typedef struct
{
    uint32_t crc;
    uint32_t master_boot_force_update;
    NS_Bank_t app1;
    NS_Bank_t app2;
    NS_Bank_t ImageUpdate;
    uint8_t public_key[64];
} NS_Bootsetting_t;
```

所以：

- 没有 `bootsetting`
- 或者 `bootsetting` 跟实际镜像不匹配

就会出现你前面遇到的那种：

- 什么都没有
- 看起来像“工程坏了”

其实只是启动链路不完整。

---

## 7. app_ota 做什么

`app_ota` 是“平时你真正运行的应用层”。

它的工作不是自己改 Flash，而是：

- 正常启动
- 蓝牙广播
- 和手机建立 OTA 交互
- 在需要时把升级任务交给 `ImageUpdate`

从产品视角看，`app_ota` 就像“用户正在用的固件”，  
而 `image_update` 更像“专门负责升级的维修工”。

---

## 8. image_update 做什么

`image_update` 不是长期常驻主应用。  
它更像一个中间层，只有在升级流程里才会真正发挥作用。

它的一个关键动作是：

- 升级完成后，重写 bootsetting
- 把激活权切回正常应用

这样下次重启时，`masterboot` 就会重新跳回 `APP1` 或 `APP2`。

---

## 9. app_ota 为什么有 APP1 和 APP2

这是 OTA 里最像“产品级设计”的地方。

`app_ota` 不是一份单独镜像，而是：

- `APP1`
- `APP2`

两个槽位。

这意味着：

- 当前运行的是 `APP1` 时，可以把新版本写到 `APP2`
- 当前运行的是 `APP2` 时，可以把新版本写到 `APP1`

这样就避免了“边运行边覆盖自己”的问题。

这就是所谓的 **双 Bank OTA**。

---

## 10. 我整理后的 target

在当前工程里，`app_ota` 已经被整理成两个可读性更好的目标：

- `N32WB031_STB_OTA_APP1`
- `N32WB031_STB_OTA_APP2`

其中：

- `APP1` 对应地址 `0x01004000`
- `APP2` 对应地址 `0x01020000`

---

## 11. 第一次为什么“烧了三个工程还是没反应”

这是你前面已经遇到过的典型坑。

原因其实不复杂：

- 你烧了 `masterboot`
- 也烧了 `app_ota`
- 也烧了 `image_update`

但如果 **没有正确配套的 `bootsetting.bin`**，那么 `masterboot` 根本不知道该跳谁。

这也是为什么 OTA 这套东西，不能简单理解成“把三个 hex 都下载一下”。

---

## 12. 第一次最小验证，最稳的方式

第一次验证，不要急着上自己改过的镜像，最稳的方式是先证明“静态链路没问题”。

推荐顺序：

1. `MasterBoot.bin`
2. `bootsetting.bin`
3. `APP1.bin`
4. `ImageUpdate.bin`

也就是先把：

- 启动入口
- 配置区
- 主应用
- 升级执行层

作为一整套放进去。

---

## 13. 官方 SDK 自带了哪些参考镜像

SDK 本身已经给了参考镜像目录：

[utilities/dfu/Image](/d:/n32wb031/sdk/N32WB03x_SDK_V2.0.0/N32WB03x_SDK_V2.0.0/utilities/dfu/Image)

其中包括：

- `MasterBoot.bin`
- `bootsetting.bin`
- `APP1.bin`
- `APP2.bin`
- `ImageUpdate.bin`

这也是第一次验证最方便的基线。

`[截图位 3：SDK 自带 Image 目录截图]`

---

## 14. 官方工具链是怎么配套的

这套 OTA 工程不只是代码，还配了一套官方工具：

### 14.1 NSUtil

位置：

[NSUtil.exe](/d:/n32wb031/sdk/N32WB03x_SDK_V2.0.0/N32WB03x_SDK_V2.0.0/utilities/dfu/NSUtil/NSUtil.exe)

它主要负责两类事情：

1. 生成 `bootsetting.bin`
2. 合并多个镜像或生成 OTA 包

比如：

- `bootsetting generate`
- `pack mergebin`
- `pack ota`

### 14.2 NSpyocd

位置：

[NSpyocd.exe](/d:/n32wb031/sdk/N32WB03x_SDK_V2.0.0/N32WB03x_SDK_V2.0.0/utilities/dfu/NSpyocd/NSpyocd.exe)

它本质上是官方打包的一套烧录工具，适合：

- 擦除芯片
- 烧录 hex / bin
- 配合 `DAPLink / CMSIS-DAP`

对你来说，它最大的价值就是：

**用 DAPLink 直接烧 OTA 合并镜像。**

---

## 15. 我额外给你做了什么

为了让这套工程更适合教学和实测，我额外帮你做了这些整理：

1. 把 OTA 三件套整理到单独目录
2. 补回了 `common/`
3. 调整了工程名
4. 给 `APP1 / APP2` 目标命名
5. 关掉了容易妨碍首次编译的部分 post-build 动作
6. 额外生成了第一次上电验证镜像
7. 给你写了 DAPLink 烧录脚本

---

## 16. 第一次上电验证镜像是什么

我专门帮你在仓库里生成过一份“第一次最小验证”镜像：

- [first_boot_verify.bin](/d:/n32wb031/template/N32WB031_STB_OTA/output/first_boot_verify.bin)
- [first_boot_verify.hex](/d:/n32wb031/template/N32WB031_STB_OTA/output/first_boot_verify.hex)

这份镜像合并了：

- `MasterBoot`
- `bootsetting`
- `APP1`
- `ImageUpdate`

它的目的不是完成升级，而是先验证：

- 能不能正常启动
- 能不能进应用
- BLE 能不能广播

---

## 17. DAPLink 烧录脚本怎么用

你现在是 `DAPLink`，所以我专门又给你做了一份脚本：

[repack_and_flash_app1_daplink.bat](/d:/n32wb031/template/N32WB031_STB_OTA/scripts/repack_and_flash_app1_daplink.bat)

它做的事情是：

1. 把当前编译出来的 `AXF` 转成 `BIN`
2. 重新生成匹配的 `bootsetting.bin`
3. 合并成新的 OTA 初始镜像
4. 用 `NSpyocd` 整片擦除
5. 再用 `NSpyocd` 烧进去

也就是说，你后面只要：

- 先重新编译
- 再运行这个脚本

就能把你当前修改过的 `APP1` 真正烧进去。

## 18. 这个脚本为什么要重新生成 bootsetting

因为 `bootsetting` 里保存了：

- 镜像版本
- 镜像大小
- 镜像 CRC
- 激活标志

如果你改了 `APP1`，但还沿用旧的 `bootsetting.bin`，就可能出现：

- 镜像确实烧进去了
- 但 `masterboot` 不认它

所以这个脚本最大的意义，不只是“帮你烧录”，而是：

**保证 `bootsetting` 和你当前编译产物匹配。**

---

## 19. 第一次成功后，你应该看到什么

你前面已经验证出来，链路打通后，理想现象会是：

- 串口输出：

  `application 1 start new ...`

- 协议栈输出：

  `APP_BLE_OS_READY`

- 板上 LED 先闪再有稳定状态

这就说明：

- `masterboot` 生效
- `bootsetting` 生效
- 成功跳进了 `APP1`
- BLE 也正常起来了

`[截图位 4：串口日志里出现 application 1 start new]`
`[截图位 5：串口日志里出现 APP_BLE_OS_READY]`

---

## 20. 为了更容易观察，我又做了什么

我还把 `APP1` 稍微改得更“有肉眼反馈”：

- 蓝牙广播名改成了更容易辨认的版本
- 让启动完成后 `LED1` 更容易被看见
- 让连接状态可以通过 `LED2` 区分

这类改动在测评里非常有用，因为 OTA 工程本身天生不像 HID、OLED 那样直观。

---

## 21. 后面真正做 OTA 升级时会发生什么

真正进入 OTA 升级验证后，逻辑会是：

1. 当前运行 `APP1`
2. 手机发起升级
3. 新镜像写入另一个区域
4. 必要时跳 `ImageUpdate`
5. `bootsetting` 被更新
6. 重启
7. 最终切到新的应用槽位

如果做得更清楚一些，最理想的后续效果应该是：

- 当前广播 `N32WB031_OTA_A1`
- 升级后重启
- 最终广播 `N32WB031_OTA_A2`

这样你肉眼和手机都能直接看出“确实切换版本了”。

---

## 22. 下一步该怎么做

现在你已经完成的是：

- 三件套工程编译通过
- 启动链路第一次打通
- `APP1` 能正常跑起来

接下来真正值得做的，就是：

1. 重新编译当前修改过的 `APP1`
2. 跑 `repack_and_flash_app1_daplink.bat`
3. 确认广播名和可见状态改动都生效
4. 然后再编 `APP2`
5. 生成 OTA 升级包
6. 做“从 A1 升到 A2”的完整升级验证

---

## 23. 这一节最重要的知识点

如果你只记住三句话，这一章就没白看：

1. **OTA 不是一个工程，而是一整套启动体系。**
2. **bootsetting 是 OTA 是否能启动的关键。**
3. **masterboot 不负责业务，它只负责判断跳谁。**

---

## 24. 小结

这套 OTA 工程真正难的地方，不在代码本身，而在于：

- 你得先理清角色
- 再理清地址
- 再理清烧录顺序
- 最后才是升级动作

只要把这几层关系捋顺，OTA 其实就不再神秘。  
它本质上就是一套“谁启动谁、谁升级谁、升级完再跳谁”的链路管理问题。

---

## 25. 手机端 OTA 工具是什么

这一步很多人最容易卡住，因为会下意识去找通用 BLE 调试工具，比如：

- `nRF Connect`
- `LightBlue`

这些工具虽然适合看广播和服务，但它们**不等于 OTA 专用升级工具**。

针对这套官方 OTA 例程，SDK 本地已经自带了安卓升级工具：

[NSAndroidUtilV1.5.apk](/d:/n32wb031/sdk/N32WB03x_SDK_V2.0.0/N32WB03x_SDK_V2.0.0/utilities/dfu/NSAndroidUtil/NSAndroidUtilV1.5.apk)

所以这一步不需要再去网上乱找。  
优先就用 SDK 自带这份 APK。

`[截图位 6：NSAndroidUtilV1.5.apk 文件位置截图]`

---

## 26. 升级包选哪个文件

当前我们已经生成好了 OTA 升级包：

[ota_dual_bank_v2.zip](/d:/n32wb031/template/N32WB031_STB_OTA/output/ota_dual_bank_v2.zip)

**手机端升级时要选整个 `zip` 文件。**

不要选里面的：

- `APP1.bin`
- `APP2.bin`
- `ImageUpdate.bin`

因为 OTA 升级包不是单纯只看镜像本体，它还依赖：

- `config.txt`
- `dfu_setting.dat`

这些文件我已经确认就在压缩包里。

也就是说：

> App 端应该导入整包 `ota_dual_bank_v2.zip`，而不是压缩包里的某个单独 `.bin`。

`[截图位 7：ota_dual_bank_v2.zip 文件位置截图]`

---

## 27. 手机端升级前先确认什么

在真正点击升级之前，建议先确认下面几件事：

1. 板子当前跑的是 `APP1`
   - 串口能看到：
     - `application 1 start new ...`
     - `APP_BLE_OS_READY`

2. 手机能搜到 OTA 设备
   - 当前建议观察名字：
     - `N32WB031_OTA_A1`

3. 板子蓝牙连接正常
   - 如果我给你的可视化版本已经烧进去
   - 连接后 `LED2` 会亮

如果上面三件事不成立，就不要急着点升级。

---

## 28. 手机端 OTA 操作步骤

下面是最推荐的手机端操作顺序：

### 第一步：安装 OTA 工具

把下面这个 APK 安装到安卓手机：

[NSAndroidUtilV1.5.apk](/d:/n32wb031/sdk/N32WB03x_SDK_V2.0.0/N32WB03x_SDK_V2.0.0/utilities/dfu/NSAndroidUtil/NSAndroidUtilV1.5.apk)

### 第二步：让板子停在 APP1 运行状态

也就是确保它当前已经能正常启动、广播、连接。

### 第三步：打开手机端 OTA 工具

进入扫描界面，搜索当前 OTA 设备。

优先找：

- `N32WB031_OTA_A1`

如果你暂时还没把可区分版本烧进去，可能看到的是旧名字。  
但只要确认当前运行的是 OTA 应用即可。

### 第四步：连接 OTA 设备

连接成功后，再进入文件选择界面。

### 第五步：选择 OTA 升级包

选择：

[ota_dual_bank_v2.zip](/d:/n32wb031/template/N32WB031_STB_OTA/output/ota_dual_bank_v2.zip)

### 第六步：开始升级

启动升级后，观察：

- 手机端进度条
- 板子是否有状态变化
- 升级完成后是否自动重启

`[截图位 8：手机端搜索到 OTA 设备]`
`[截图位 9：手机端连接成功界面]`
`[截图位 10：手机端选择 ota_dual_bank_v2.zip]`
`[截图位 11：手机端 OTA 升级进度界面]`

---

## 29. 升级成功后该看什么

如果升级真正成功，最关键的现象不是“App 弹了一个成功提示”，而是：

### 1. 板子重启

升级完成后，板子通常会自动重启。

### 2. 串口日志变化

原来运行 `APP1` 时是：

```text
application 1 start new ...
```

如果切到 `APP2` 成功，重启后应该变成：

```text
application 2 start new ...
```

### 3. 蓝牙名字变化

如果我前面改的“区分 APP1 / APP2 广播名”版本已经烧进去，那么：

- 升级前应该搜到：`N32WB031_OTA_A1`
- 升级后应该搜到：`N32WB031_OTA_A2`

这一步是最直观的成功标志。

`[截图位 12：升级前串口日志 application 1]`
`[截图位 13：升级后串口日志 application 2]`
`[截图位 14：升级前后蓝牙名字变化对比图]`

---

## 30. 为什么不是直接烧 APP2

这也是 OTA 测试里非常值得单独强调的一点。

如果你的目标是：

**验证 OTA 机制**

那就不应该简单地把 `APP2` 直接烧进去运行。  
因为那样只能证明：

- `APP2` 本身能跑

却不能证明：

- 手机端 OTA 包生效
- 板子写入了新镜像
- `bootsetting` 被更新
- `masterboot` 下次启动真的切换到了新槽位

所以真正有意义的 OTA 验证应该是：

> 保持板子先跑在 `APP1`，然后通过手机蓝牙把它升级到 `APP2`。

---

## 31. 常见问题

### 31.1 手机端找不到 OTA 工具

不用网上找，SDK 本地已经带了：

[NSAndroidUtilV1.5.apk](/d:/n32wb031/sdk/N32WB03x_SDK_V2.0.0/N32WB03x_SDK_V2.0.0/utilities/dfu/NSAndroidUtil/NSAndroidUtilV1.5.apk)

### 31.2 升级包该选哪个

选整个：

[ota_dual_bank_v2.zip](/d:/n32wb031/template/N32WB031_STB_OTA/output/ota_dual_bank_v2.zip)

不是里面的单独 `.bin`。

### 31.3 升级后还是 application 1

说明：

- 升级包没有真正写成功
- 或者 `bootsetting` 没有正确切换激活槽位

### 31.4 升级成功但蓝牙名字没变

这要分两种情况：

1. 如果当前烧进去的是我改过广播名的版本，那应该会看到 `A1/A2` 的变化
2. 如果你烧进去的还是官方原版 OTA 应用，那升级成功后蓝牙名可能还是旧名字

所以最硬的判断仍然是看串口日志里的：

- `application 1`
- `application 2`

---

## 32. 这一节最重要的知识点

如果你只记住两句话，这一章就够用了：

1. **手机端 OTA 工具用官方 SDK 自带的 `NSAndroidUtilV1.5.apk`**
2. **升级时要选整个 `ota_dual_bank_v2.zip`，不是单独某个 bin**

---

## 33. 小结

到这里，这套 OTA 的实验路径已经比较完整了：

1. 三件套工程编译通过
2. `bootsetting` 和分区关系理清
3. 初始镜像烧进去，成功跑起 `APP1`
4. 生成 OTA 升级包
5. 手机端使用官方安卓工具发起蓝牙升级

也就是说，后面真正的验证重点只剩下一个：

**升级后，板子是否从 `APP1` 真正切到 `APP2`。**
