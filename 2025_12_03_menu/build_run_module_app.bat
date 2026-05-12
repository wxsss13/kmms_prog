@echo off
chcp 1251 > log
del log

set CPP_FILES="menu_items.cpp menu_function.cpp main.cpp"
set EXE=menu.exe
set CHARSET="-finput-charset=utf-8 -fexec-charset=windows-1251"

if exist %EXE% del %EXE%

g++ "%CHARSET%" "%CPP_FILES%" -o %EXE%

%EXE%
