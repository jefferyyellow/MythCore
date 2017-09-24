@echo off
set TortoiseSVN=C:\Program Files\TortoiseSVN\bin\TortoiseProc.exe
set VC_BUILD_PATH=C:\Program Files\Microsoft Visual Studio 8\VC\vcpackages
set SLN_PATH=E:\TemplateUpdate\tools\TemplateEditor
set UPDATE_PATH=E:\TemplateUpdate

"%TortoiseSVN%" /command:update /path:"%UPDATE_PATH%\common*%UPDATE_PATH%\tools" /closeonend:2

if exist "%SLN_PATH%\debug\TemplateEditor.exe" (
del/f/s/q %SLN_PATH%\debug\TemplateEditor.exe
)

:: ����vcbuild ·��
C:
cd %VC_BUILD_PATH%
vcbuild.exe /rebuild %SLN_PATH%\TemplateEditor.sln "Debug|Win32"
vcbuild.exe %SLN_PATH%\TemplateEditor.sln "Debug|Win32"

if not exist "%SLN_PATH%\debug\TemplateEditor.exe" (echo "Compile Error !") else (copy %SLN_PATH%\debug\TemplateEditor.exe E:\ð����2�߻��ĵ�\ģ������)
call "E:\ð����2�߻��ĵ�\ģ������\update_new.bat"
pause