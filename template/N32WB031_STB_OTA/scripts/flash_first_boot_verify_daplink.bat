@echo off
setlocal

for %%I in ("%~dp0..") do set ROOT=%%~fI\

set TOOL=%ROOT%..\..\sdk\N32WB03x_SDK_V2.0.0\N32WB03x_SDK_V2.0.0\utilities\dfu\NSpyocd\NSpyocd.exe
set IMAGE=%ROOT%output\first_boot_verify.hex
set TARGET=n32wb031

echo.
echo [1/2] Erasing chip with DAPLink...
"%TOOL%" erase --chip -M attach -t %TARGET%
if errorlevel 1 goto fail

echo.
echo [2/2] Programming %IMAGE% ...
"%TOOL%" load -M attach -t %TARGET% "%IMAGE%"
if errorlevel 1 goto fail

echo.
echo Programming finished.
echo Power cycle or reset the board, then observe LED / UART / BLE behavior.
goto end

:fail
echo.
echo Programming failed.
echo Please check DAPLink connection and retry.

:end
endlocal
pause
