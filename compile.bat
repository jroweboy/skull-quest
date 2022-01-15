@echo off

set name="skullquest"

set path="c:\cc65\bin\"

set CC65_HOME=..\

cc65 -Oirs %name%.c --add-source
ca65 crt0.s
if not exist %name%.s exit 7
ca65 %name%.s -g

ld65 -C nrom_32k_vert.cfg -o %name%.nes crt0.o %name%.o nes.lib -Ln labels.txt

if %ERRORLEVEL% EQU 0 (
    del *.o

    move /Y labels.txt BUILD\ 
    move /Y %name%.s BUILD\ 
    move /Y %name%.nes BUILD\ 

    pause

    BUILD\%name%.nes
)
