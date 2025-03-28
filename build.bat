@echo off
setlocal enabledelayedexpansion

:: Проверка наличия Qt
if not exist "C:\Qt\5.15.2\mingw81_64\bin" (
    echo Ошибка: Qt не найден. Убедитесь, что Qt установлен в C:\Qt\5.15.2\mingw81_64
    pause
    exit /b 1
)

:: Проверка наличия MinGW
if not exist "C:\Qt\Tools\mingw810_64\bin" (
    echo Ошибка: MinGW не найден. Убедитесь, что MinGW установлен в C:\Qt\Tools\mingw810_64
    pause
    exit /b 1
)

:: Установка путей
set PATH=C:\Qt\5.15.2\mingw81_64\bin;C:\Qt\Tools\mingw810_64\bin;%PATH%

echo Начинаем сборку проекта...

:: Создание директории для сборки
if not exist build (
    echo Создание директории build...
    mkdir build
)
cd build

:: Очистка предыдущей сборки
echo Очистка предыдущей сборки...
if exist Makefile mingw32-make clean
if exist Makefile.Debug del /f /q Makefile.Debug
if exist Makefile.Release del /f /q Makefile.Release
if exist .qmake.stash del /f /q .qmake.stash

:: Генерация файлов проекта
echo Генерация файлов проекта...
qmake ..\Taymer.pro -spec win32-g++
if errorlevel 1 (
    echo Ошибка при генерации файлов проекта
    cd ..
    pause
    exit /b 1
)

:: Сборка
echo Сборка проекта...
mingw32-make
if errorlevel 1 (
    echo Ошибка при сборке проекта
    cd ..
    pause
    exit /b 1
)

:: Копирование зависимостей
echo Копирование зависимостей...
windeployqt release\Taymer.exe
if errorlevel 1 (
    echo Ошибка при копировании зависимостей
    cd ..
    pause
    exit /b 1
)

cd ..

:: Создание директории для установщика
if not exist installer (
    echo Создание директории installer...
    mkdir installer
)
if not exist installer\Output (
    mkdir installer\Output
)

:: Копирование файлов для установщика
echo Копирование файлов для установщика...
xcopy /Y /E build\release\*.* installer\files\

:: Проверка наличия Inno Setup
if not exist "C:\Users\andrewdumin\AppData\Local\Programs\Inno Setup 6\ISCC.exe" (
    echo Ошибка: Inno Setup не найден. Убедитесь, что Inno Setup установлен
    pause
    exit /b 1
)

:: Создание установщика
echo Создание установщика...
"C:\Users\andrewdumin\AppData\Local\Programs\Inno Setup 6\ISCC.exe" setup.iss
if errorlevel 1 (
    echo Ошибка при создании установщика
    pause
    exit /b 1
)

echo.
echo Сборка успешно завершена!
echo Исполняемый файл находится в папке build\release
echo Установщик находится в папке installer\Output
echo.

:: Копируем exe в корневую папку для удобства
echo Копирование exe в корневую папку...
copy /Y build\release\Taymer.exe .

pause 