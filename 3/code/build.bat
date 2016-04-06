@echo off
mkdir ..\build
pushd ..\build

clang ../code/glplay.c -IC:/dev/GLEW/include -IC:/dev/GLFW/include -lC:/dev/GLEW/lib/Release/x64/glew32.lib -lC:/dev/GLFW/lib/glfw3.lib -lUser32.lib -lGdi32.lib -lShell32.lib -lOpengl32.lib -lMSVCRTD.lib -llibcmt.lib -o glplay.exe

popd
