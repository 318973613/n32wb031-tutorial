# N32WB031 OTA 升级工程说明

## 1. OTA 不是一个工程

这套 OTA 不是“打开一个工程编译下载就结束”，而是由三类工程共同组成：

1. `masterboot`
   - 负责上电启动、判断当前激活镜像、必要时跳转
   - 也负责串口进入升级流程

2. `app_ota`
   - 正常运行的蓝牙 OTA 应用
   - 负责在手机侧完成升级交互
   - 有两个目标：`APP1` 和 `APP2`

3. `image_update`
   - 升级过程中的中间镜像层
   - 用于完成镜像更新动作

4. `common`
   - 这不是单独烧录的工程
   - 但三件套都依赖这里的公共 DFU 文件

## 2. 当前整理后的工程位置

当前我已经整理到：

- [template/N32WB031_STB_OTA](/d:/n32wb031/template/N32WB031_STB_OTA)
- [README.txt](/d:/n32wb031/template/N32WB031_STB_OTA/README.txt)

其中主工程分别是：

- [N32WB031_STB_OTA_MASTERBOOT.uvprojx](/d:/n32wb031/template/N32WB031_STB_OTA/masterboot/MDK-ARM/N32WB031_STB_OTA_MASTERBOOT.uvprojx)
- [N32WB031_STB_OTA_APP.uvprojx](/d:/n32wb031/template/N32WB031_STB_OTA/app_ota/MDK-ARM/N32WB031_STB_OTA_APP.uvprojx)
- [N32WB031_STB_OTA_IMAGE_UPDATE.uvprojx](/d:/n32wb031/template/N32WB031_STB_OTA/image_update/MDK-ARM/N32WB031_STB_OTA_IMAGE_UPDATE.uvprojx)

## 3. 分区地址关系

根据工程生成的 `.sct` 文件和 `ns_dfu_boot.h` 中的定义，当前 OTA 分区关系如下：

| 区域 | 起始地址 | 大小 | 作用 |
| --- | --- | --- | --- |
| `MASTERBOOT` | `0x01000000` | `0x00002000` | 启动入口 |
| `BOOTSETTING` | `0x01002000` | `0x00001000` | 启动配置、激活标志、CRC 信息 |
| `APP_DATA` | `0x01003000` | `0x00001000` | 应用数据区 |
| `APP1` | `0x01004000` | `0x0001C000` | 主应用槽位 1 |
| `APP2` | `0x01020000` | `0x0001C000` | 主应用槽位 2 |
| `IMAGE_UPDATE` | `0x0103C000` | `0x00004000` | 升级执行层 |

从布局上看，逻辑很清楚：

- 最前面是 `masterboot`
- 中间是两份应用镜像 `APP1 / APP2`
- 最后是 `image_update`

## 4. 三个工程当前对应到哪个分区

### 4.1 masterboot

从 `masterboot` 的散装文件可以确认：

- 起始地址：`0x01000000`
- 大小：`0x2000`

对应分区：

- `MASTERBOOT`

### 4.2 image_update

从 `image_update` 的 `.sct` 可以确认：

- 起始地址：`0x0103C000`
- 大小：`0x4000`

对应分区：

- `IMAGE_UPDATE`

### 4.3 app_ota

`app_ota` 是双目标工程：

- `N32WB031_STB_OTA_APP1`
  - 起始地址：`0x01004000`
  - 对应 `APP1`

- `N32WB031_STB_OTA_APP2`
  - 起始地址：`0x01020000`
  - 对应 `APP2`

也就是说，这份工程本身不是一份镜像，而是两份“同一套 OTA 应用，不同烧录槽位”的构建目标。

## 5. 现在你手上已经确认编过的产物

目前至少已经确认存在：

### masterboot 产物

- `N32WB031_STB_OTA_MASTERBOOT.axf`
- `N32WB031_STB_OTA_MASTERBOOT.hex`

位置：

`template/N32WB031_STB_OTA/masterboot/MDK-ARM/Objects/`

### app_ota 产物

- `N32WB031_STB_OTA_APP1.axf`
- `N32WB031_STB_OTA_APP1.hex`

位置：

`template/N32WB031_STB_OTA/app_ota/MDK-ARM/Objects/`

### image_update 产物

- `N32WB031_STB_OTA_IMAGE_UPDATE.axf`
- `N32WB031_STB_OTA_IMAGE_UPDATE.hex`

位置：

`template/N32WB031_STB_OTA/image_update/MDK-ARM/Objects/`

## 6. masterboot 到底怎么决定跳转谁

`masterboot` 的逻辑核心是：

1. 读取 `bootsetting`
2. 检查 `APP1 / APP2 / IMAGE_UPDATE` 的激活标志和 CRC
3. 谁合法、谁激活，就跳谁

简单理解就是：

- 平时优先跳当前激活的应用
- 升级过程中如果设置了 `ImageUpdate` 激活，就先跳 `image_update`
- 升级完成后，再把激活权切回 `APP1` 或 `APP2`

所以 OTA 的关键并不是“重新烧掉整片程序”，而是：

**通过更新 bootsetting，让启动入口跳转到正确镜像。**

## 7. 推荐烧录顺序

第一次搭起 OTA 环境时，推荐顺序如下：

1. 先烧 `masterboot`
2. 再烧 `image_update`
3. 再烧 `app_ota APP1`

如果后面你要完整验证双 Bank OTA，再补：

4. 编译并准备 `APP2`

这样做的原因是：

- `masterboot` 是底座，必须先有
- `image_update` 是 OTA 执行层，也必须先在
- `APP1` 是先跑起来的主应用
- `APP2` 可以留到后面作为升级目标

## 8. 第一次最小验证目标

第一次验证，不要一上来就真的 OTA 升级，先证明最小闭环：

1. `masterboot` 能正常启动
2. 最终能跳到 `app_ota`
3. `app_ota` 能正常广播蓝牙名字
4. 手机能搜到 OTA 设备

只要这四步通了，说明“静态启动链路”已经是对的。

## 9. 你接下来最推荐做的动作

现在建议你按这个顺序继续：

1. 先烧 `masterboot`
2. 再烧 `image_update`
3. 再烧 `APP1`
4. 上电观察实际现象

观察点包括：

- 板载 LED 行为
- 是否能正常启动
- 是否能搜到 OTA 蓝牙设备

## 10. 后面还要做什么

这份说明解决的是：

- 地址关系
- 工程角色
- 烧录顺序

下一步真正进入 OTA 实战时，还需要继续补两部分：

1. 手机端 OTA 操作流程
2. 升级成功后的现象判断

也就是从“工程准备阶段”进入“实测阶段”。
