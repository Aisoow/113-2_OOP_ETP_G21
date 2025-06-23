@echo off
title 貪吃蛇遊戲啟動器
color 0A

:: 設定 UTF-8 編碼
chcp 65001 > nul

echo.
echo ╔══════════════════════════════════════════╗
echo ║            🐍 貪吃蛇遊戲啟動器            ║
echo ╚══════════════════════════════════════════╝
echo.

:: 獲取當前批次檔路徑
set "SCRIPT_DIR=%~dp0"
set "GAME_DIR=%SCRIPT_DIR%Game"
set "GAME_EXE=%GAME_DIR%\SnakeGame.exe"

echo 檢查遊戲檔案...
if exist "%GAME_EXE%" (
    echo ✓ 找到遊戲檔案: %GAME_EXE%
    echo.
    echo 正在啟動遊戲...
    cd /d "%GAME_DIR%"
    "%GAME_EXE%"
    
    if errorlevel 1 (
        echo.
        echo ❌ 遊戲執行時發生錯誤
    ) else (
        echo.
        echo ✓ 遊戲正常結束
    )
) else (
    echo ❌ 找不到遊戲檔案
    echo 預期位置: %GAME_EXE%
    echo.
    echo 請確認檔案結構：
    echo %SCRIPT_DIR%
    echo ├── 啟動遊戲.bat ^(此檔案^)
    echo └── Game\
    echo     └── SnakeGame.exe
)

echo.
echo 按任意鍵退出...
pause > nul