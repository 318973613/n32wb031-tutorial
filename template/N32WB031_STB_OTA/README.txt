N32WB031 OTA Workspace

This folder is organized for readers who want to learn the OTA flow step by step.

Structure:

1. masterboot/
   - Bootloader layer.
   - First code that runs after reset.

2. app_ota/
   - Main BLE OTA application.
   - Contains APP1 / APP2 targets.

3. image_update/
   - Upgrade execution layer.

4. common/
   - Shared DFU helper source files.

5. docs/
   - Reading materials.
   - Start with:
     a. OTA升级工程说明.md
     b. OTA第一次上电验证.md
     c. OTA升级包生成说明.md
     d. OTA蓝牙升级教程.md

6. scripts/
   - Helper scripts.
   - flash_first_boot_verify_daplink.bat
   - repack_and_flash_app1_daplink.bat
   - generate_ota_package.bat

7. output/
   - Generated binaries, merged images, OTA zip package.

Suggested reading order:

1. docs/OTA升级工程说明.md
2. docs/OTA第一次上电验证.md
3. Build masterboot / APP1 / APP2 / image_update
4. scripts/repack_and_flash_app1_daplink.bat
5. docs/OTA升级包生成说明.md
6. scripts/generate_ota_package.bat
7. docs/OTA蓝牙升级教程.md

Suggested practical order:

1. Compile all three project groups.
2. Flash first_boot_verify or repack_and_flash_app1_daplink.
3. Confirm APP1 boots and advertises.
4. Generate ota_dual_bank_v2.zip.
5. Upgrade from APP1 to APP2 with the Android OTA app.
