cd /d "C:\Users\Administrator\source\repos\AntivirusService\AntivirusService" &msbuild "AntivirusService.vcxproj" /t:sdvViewer /p:configuration="Release" /p:platform=Win32
exit %errorlevel% 