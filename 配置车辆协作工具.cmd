@echo off
chcp 65001 >nul
cd /d "%~dp0"
python tools\model_review\setup_local.py
if errorlevel 1 goto end
echo 配置完成。本机可使用 MATLAB 模型比较与三方合并。
:end
pause
