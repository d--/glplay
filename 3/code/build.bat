@echo off
mkdir ..\build
pushd ..\build

cl -Zi -MD ..\code\glplay.c /IC:\dev\GLFW\include /IC:\dev\GLEW\include C:\dev\GLFW\lib\glfw3.lib C:\dev\GLEW\lib\Release\x64\glew32.lib User32.lib Gdi32.lib Shell32.lib Opengl32.lib

popd
