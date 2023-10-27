@echo off
cls

SET work_dir=%~dp0

cl /std:c++20 /Z7 /Fo"%work_dir%/bin/" ^
 %work_dir%/main.cpp ^
 %work_dir%/server.cpp
