@echo off
setlocal

for %%I in ("%~dp0..") do set ROOT=%%~fI\
set FROMELF=D:\51\ARM\ARMCOMPLIER506\bin\fromelf.exe
set NSUTIL=%ROOT%..\..\sdk\N32WB03x_SDK_V2.0.0\N32WB03x_SDK_V2.0.0\utilities\dfu\NSUtil\NSUtil.exe
set PRIVKEY=%ROOT%..\..\sdk\N32WB03x_SDK_V2.0.0\N32WB03x_SDK_V2.0.0\utilities\dfu\keys\private_key.pem

set APP1_AXF=%ROOT%app_ota\MDK-ARM\Objects\N32WB031_STB_OTA_APP1.axf
set APP2_AXF=%ROOT%app_ota\MDK-ARM\Objects\N32WB031_STB_OTA_APP2.axf
set IU_AXF=%ROOT%image_update\MDK-ARM\Objects\N32WB031_STB_OTA_IMAGE_UPDATE.axf

set APP1_BIN=%ROOT%output\APP1.bin
set APP2_BIN=%ROOT%output\APP2.bin
set IU_BIN=%ROOT%output\ImageUpdate.bin

set OTA_OUT=%ROOT%output\ota_dual_bank_v2

echo.
echo [1/2] Converting AXF to BIN...
"%FROMELF%" --bin --output="%APP1_BIN%" "%APP1_AXF%"
if errorlevel 1 goto fail
"%FROMELF%" --bin --output="%APP2_BIN%" "%APP2_AXF%"
if errorlevel 1 goto fail
"%FROMELF%" --bin --output="%IU_BIN%" "%IU_AXF%"
if errorlevel 1 goto fail

echo.
echo [2/2] Packaging OTA file...
"%NSUTIL%" pack otawithsigning "%OTA_OUT%" ^
  --private_key "%PRIVKEY%" ^
  --app1_start_address 0x01004000 ^
  --app1_version 0x00000001 ^
  --app1_bin "%APP1_BIN%" ^
  --app2_start_address 0x01020000 ^
  --app2_version 0x00000002 ^
  --app2_bin "%APP2_BIN%" ^
  --image_update_start_address 0x0103C000 ^
  --image_update_version 0x00000001 ^
  --image_update_bin "%IU_BIN%"
if errorlevel 1 goto fail

echo.
echo OTA package generated.
echo Output base: %OTA_OUT%
goto end

:fail
echo.
echo Failed to generate OTA package.

:end
endlocal
pause
