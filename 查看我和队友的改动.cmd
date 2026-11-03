@echo off
chcp 65001 >nul
cd /d "%~dp0"
python tools\model_review\run_review.py
pause
