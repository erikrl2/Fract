@echo off
pushd %~dp0\..
call vendor\Premake\bin\premake5-win.exe vs2022
popd
