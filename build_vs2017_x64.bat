@echo off

if not exist build mkdir build

pushd build
cmake -G "Visual Studio 15 2017 Win64" ..
popd

if %errorlevel%==0 goto end

pause

:end
