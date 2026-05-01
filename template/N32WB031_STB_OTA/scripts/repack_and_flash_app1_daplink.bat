@echo off
setlocal

for %%I in ("%~dp0..") do set ROOT=%%~fI\
set FROMELF=D:\51\ARM\ARMCOMPLIER506\bin\fromelf.exe
set NSUTIL=%ROOT%..\..\sdk\N32WB03x_SDK_V2.0.0\N32WB03x_SDK_V2.0.0\utilities\dfu\NSUtil\NSUtil.exe
set NSPYOCD=%ROOT%..\..\sdk\N32WB03x_SDK_V2.0.0\N32WB03x_SDK_V2.0.0\utilities\dfu\NSpyocd\NSpyocd.exe
set PUBKEY=%ROOT%..\..\sdk\N32WB03x_SDK_V2.0.0\N32WB03x_SDK_V2.0.0\utilities\dfu\keys\public_key.bin

set MB_AXF=%ROOT%masterboot\MDK-ARM\Objects\N32WB031_STB_OTA_MASTERBOOT.axf
set APP1_AXF=%ROOT%app_ota\MDK-ARM\Objects\N32WB031_STB_OTA_APP1.axf
set IU_AXF=%ROOT%image_update\MDK-ARM\Objects\N32WB031_STB_OTA_IMAGE_UPDATE.axf

set MB_BIN_FILE=%ROOT%output\MasterBoot.bin
set APP1_BIN_FILE=%ROOT%output\APP1.bin
set IU_BIN_FILE=%ROOT%output\ImageUpdate.bin
set BOOT_BIN_FILE=%ROOT%output\bootsetting.bin

set MB_BIN_BASE=%ROOT%output\MasterBoot
set APP1_BIN_BASE=%ROOT%output\APP1
set IU_BIN_BASE=%ROOT%output\ImageUpdate
set BOOT_BIN_BASE=%ROOT%output\bootsetting
set MERGED_OUT=%ROOT%output\first_boot_verify_app1

echo.
echo [1/5] Converting AXF to BIN...
"%FROMELF%" --bin --output="%MB_BIN_FILE%" "%MB_AXF%"
if errorlevel 1 goto fail
"%FROMELF%" --bin --output="%APP1_BIN_FILE%" "%APP1_AXF%"
if errorlevel 1 goto fail
"%FROMELF%" --bin --output="%IU_BIN_FILE%" "%IU_AXF%"
if errorlevel 1 goto fail

echo.
echo [2/5] Generating bootsetting.bin ...
"%NSUTIL%" bootsetting generate "%BOOT_BIN_FILE%" ^
  --public_key_file "%PUBKEY%" ^
  --bank1_start_address 0x1004000 ^
  --bank1_version 0x00000001 ^
  --bank1_activation yes ^
  --bank1_bin "%APP1_BIN_FILE%" ^
  --image_update_start_address 0x0103C000 ^
  --image_update_version 0x00000001 ^
  --image_update_activation no ^
  --image_update_bin "%IU_BIN_FILE%"
if errorlevel 1 goto fail

echo.
echo [3/5] Merging images...
"%NSUTIL%" pack mergebin "%MERGED_OUT%" ^
  --masterboot "%MB_BIN_BASE%" ^
  --bootsetting "%BOOT_BIN_BASE%" ^
  --app1 "%APP1_BIN_BASE%" ^
  --imageupdate "%IU_BIN_BASE%"
if errorlevel 1 goto fail

echo.
echo [4/5] Erasing chip...
"%NSPYOCD%" erase --chip -M attach -t n32wb031
if errorlevel 1 goto fail

echo.
echo [5/5] Flashing merged image...
"%NSPYOCD%" load -M attach -t n32wb031 "%MERGED_OUT%.hex"
if errorlevel 1 goto fail

echo.
echo Done. Power cycle or reset the board and observe LED / BLE behavior.
goto end

:fail
echo.
echo Failed. Please check the build outputs and DAPLink connection.

:end
endlocal
pause
