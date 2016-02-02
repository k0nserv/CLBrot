@echo off
set roaming=%APPDATA%
set folder=CLBrot
set root=%appdata%\%folder%
set kernel=kernel.c
set readme=README.txt
set screenshotFolder=screenshots


echo.
echo CLBrot installer
echo The installer will now make the nessecary changes



if %roaming% == "" (
    echo Couldn't resolve appdata folder
    exit /b
)

call:createIfNotExist %root%
call:createIfNotExist %root%\%screenshotFolder%

call:copyIfNotExist %kernel% %root%\%kernel%
call:copyIfNotExist %readme% %root%\%readme%


echo Installed succesfully

echo.
goto :eof

:createIfNotExist
if not exist %1 (
    echo Creating directory %1
    mkdir %1
)
goto :eof

:copyIfNotExist
if not exist %2 (
    echo Creating copy of %1 at %2
    copy %1 %2 > NUL
)
goto :eof

