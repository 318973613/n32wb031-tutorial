N32WB031 OTA Project Bundle

This directory contains the three core projects required for the official OTA flow:

1. masterboot
   - Project: N32WB031_STB_OTA_MASTERBOOT
   - Purpose: bootloader / serial receiving / image jump

2. app_ota
   - Project: N32WB031_STB_OTA_APP
   - Purpose: main BLE OTA application layer

3. image_update
   - Project: N32WB031_STB_OTA_IMAGE_UPDATE
   - Purpose: image update layer used during OTA process

Notes:
1. OTA is not a single-project feature. It is a coordinated workflow across these projects.
2. Build and flashing order matters.
3. Follow the official Firmware Upgrade User's Guide when performing a full OTA test.

Suggested learning order:
1. Read masterboot first
2. Then read app_ota
3. Then read image_update

Suggested target order in app_ota:
1. N32WB031_STB_OTA_APP1
2. N32WB031_STB_OTA_APP2
