# OTA 升级包生成说明

## 1. 这一步是做什么

当前板子已经能够正常启动 `APP1`。  
下一步就不是“继续烧录 APP2”，而是要做一件更接近真实 OTA 的事：

**生成一个蓝牙 OTA 升级包，让正在运行的 APP1 通过手机升级到 APP2。**

也就是说：

- 现在跑的是 `A1`
- 目标是升级到 `A2`

## 2. 用哪个脚本

我已经给你准备好了一个脚本：

[generate_ota_package.bat](/d:/n32wb031/template/N32WB031_STB_OTA/scripts/generate_ota_package.bat)

这个脚本会自动完成：

1. 把 `APP1.axf` 转成 `APP1.bin`
2. 把 `APP2.axf` 转成 `APP2.bin`
3. 把 `ImageUpdate.axf` 转成 `ImageUpdate.bin`
4. 调用官方 `NSUtil.exe`
5. 生成 OTA 升级包

## 3. 它依赖哪些文件

脚本默认使用：

- [N32WB031_STB_OTA_APP1.axf](/d:/n32wb031/template/N32WB031_STB_OTA/app_ota/MDK-ARM/Objects/N32WB031_STB_OTA_APP1.axf)
- [N32WB031_STB_OTA_APP2.axf](/d:/n32wb031/template/N32WB031_STB_OTA/app_ota/MDK-ARM/Objects/N32WB031_STB_OTA_APP2.axf)
- [N32WB031_STB_OTA_IMAGE_UPDATE.axf](/d:/n32wb031/template/N32WB031_STB_OTA/image_update/MDK-ARM/Objects/N32WB031_STB_OTA_IMAGE_UPDATE.axf)

所以在运行脚本之前，必须先保证：

- `APP1` 编译通过
- `APP2` 编译通过
- `ImageUpdate` 编译通过

## 4. 升级包里包含什么

这个脚本生成的是一份 **双 Bank OTA 包**，它会把下面几部分信息打进去：

- `APP1` 起始地址：`0x01004000`
- `APP1` 版本：`0x00000001`
- `APP2` 起始地址：`0x01020000`
- `APP2` 版本：`0x00000002`
- `ImageUpdate` 起始地址：`0x0103C000`
- `ImageUpdate` 版本：`0x00000001`

这里的设计意图是：

- 当前运行 `APP1`
- 用 OTA 包把 `APP2` 作为“新版本”送给板子

## 5. 为什么 APP2 版本号要更高

因为 OTA 逻辑通常需要知道：

- 谁是旧版本
- 谁是新版本

当前脚本里设置的是：

- `APP1` 版本：`1`
- `APP2` 版本：`2`

这样后面升级时，系统更容易判断：

> 这是一次从 `A1` 升到 `A2` 的升级，而不是反过来覆盖旧版本。

## 6. 使用方法

### 第一步
先确认这三个工程都编译通过：

- `N32WB031_STB_OTA_MASTERBOOT`
- `N32WB031_STB_OTA_APP1`
- `N32WB031_STB_OTA_APP2`
- `N32WB031_STB_OTA_IMAGE_UPDATE`

### 第二步
运行：

[generate_ota_package.bat](/d:/n32wb031/template/N32WB031_STB_OTA/scripts/generate_ota_package.bat)

### 第三步
看脚本输出是否提示：

- `Converting AXF to BIN...`
- `Packaging OTA file...`
- `OTA package generated.`

## 7. 生成后的文件看哪里

脚本输出基名目前是：

`ota_dual_bank_v2`

也就是说，生成结果应该出现在：

- `template/N32WB031_STB_OTA/`

并且文件名会围绕这个前缀展开。

`[截图位 1：运行 generate_ota_package.bat 的终端输出]`
`[截图位 2：生成后的 OTA 文件列表]`

## 8. 这一节完成后意味着什么

如果 OTA 包生成成功，就说明：

- `APP1 / APP2 / ImageUpdate` 三个构建产物齐全
- 版本信息已经准备好了
- OTA 升级输入文件已经具备

也就是说，接下来你就可以进入真正的：

**手机端蓝牙 OTA 升级验证**

## 9. 下一步会做什么

生成 OTA 包以后，下一步就是：

1. 保持板子当前运行在 `APP1`
2. 手机连接 `N32WB031_OTA_A1`
3. 选择 OTA 升级文件
4. 发起升级
5. 观察升级后是否切换成 `N32WB031_OTA_A2`

如果这一步成功，那你的 OTA 测评就真正闭环了。
