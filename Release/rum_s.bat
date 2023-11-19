"C:\Program Files (x86)\Microsoft SDKs\ClickOnce\SignTool\signtool.exe"  sign /a serial.exe

rem "C:\Program Files (x86)\Microsoft SDKs\ClickOnce\SignTool\signtool.exe"  verify  serial.exe
rem "C:\Program Files (x86)\Microsoft SDKs\ClickOnce\SignTool\signtool.exe" sign  /f Windows_TemporaryKey.pfx /t http://timestamp.digicert.com serial.exe
rem "C:\Program Files (x86)\Microsoft SDKs\ClickOnce\SignTool\signtool.exe" sign  /f OEM_Test_PK_Cert_2017.pfx /t http://timestamp.digicert.com serial.exe


"C:\Program Files (x86)\Microsoft SDKs\ClickOnce\SignTool\signtool.exe" sign /debug /p apprieu38140 /f apprieu.pfx serial.exe

"C:\Program Files (x86)\Microsoft SDKs\ClickOnce\SignTool\signtool.exe"  verify  I:\Projets\Serial\Release\serial.exe