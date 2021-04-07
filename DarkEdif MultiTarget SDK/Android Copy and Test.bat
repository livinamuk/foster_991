@echo off
SETLOCAL EnableDelayedExpansion
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do     rem"') do (
  set "DEL=%%a"
)
title Android Bluewing tester
color 07
echo Press any key for ZIP -^> Fusion ...
pause > nul
:redo
echo Copying Data ZIP -^> Fusion ...
XCOPY "X:\Fusion-related\DarkEdif MultiTarget SDK\DarkEdif MultiTarget SDK\MFX\Extensions" "D:\Program Files (x86)\Steam\steamapps\common\Clickteam Fusion 2.5\Extensions" /I /S /Y /C /H /R
XCOPY "X:\Fusion-related\DarkEdif MultiTarget SDK\DarkEdif MultiTarget SDK\MFX\Data" "D:\Program Files (x86)\Steam\steamapps\common\Clickteam Fusion 2.5\Data" /I /S /Y /C /H /R
call :colorEcho 0E "Now building with Fusion..."
echo .
"X:\Fusion-related\DarkEdif MultiTarget SDK\DarkEdif MultiTarget SDK\WaitForFusionToFinishBuilding.exe" /APKPath="C:\Users\Phi\Desktop\EXEs\template tester.apk"
call :colorEcho 0E "Deploying to device..."
echo .
echo Fusion -^> device (uninstalling)
rem adb shell pm list packages com.darkwire.templatetester > nul
rem IF %ERRORLEVEL NEQ 0
"C:\Microsoft\AndroidSDK\25\platform-tools\adb.exe" uninstall com.darkwire.templatetester > nul
echo mmf2 -^> device (pushing)
rem "C:\Microsoft\AndroidSDK\25\platform-tools\adb.exe" push "C:\Users\Phi\Desktop\EXEs\template tester.apk" "/mnt/m_internal_storage/template tester.apk"
"C:\Microsoft\AndroidSDK\25\platform-tools\adb.exe" push "C:\Users\Phi\Desktop\EXEs\template tester.apk" "/mnt/m_internal_storage/template tester.apk" > nul
echo Fusion -^> device (installing)
rem adb install doesn't work. Also note all spaces in APK name must be escaped.
"C:\Microsoft\AndroidSDK\25\platform-tools\adb.exe" shell pm install -tsrdg "/mnt/m_internal_storage/template\ tester.apk"
call :colorEcho 0E "Ready for VS debugging."
echo .
pause
clear
goto redo
rem "C:\Microsoft\AndroidSDK\25\platform-tools\adb.exe" logcat -s MMFRuntime:V DEBUG:V MMFRuntimeNative:V libc:V

:colorEcho
echo off
<nul set /p ".=%DEL%" > "%~2"
findstr /v /a:%1 /R "^$" "%~2" nul
del "%~2" > nul 2>&1i