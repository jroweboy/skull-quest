@echo off

@REM ===================================
@REM Build Configuration - update these settings as needed

@REM Name of the project
set name=skullquest

@REM Source files in the project
set srcs=%name%.c crt0.s

@REM Update path to include the CC65 libraries
set path="c:\cc65\bin\"
set CC65_HOME=..\

@REM Directory to store the build artifacts in
set out=BUILD

@REM Linker script - change if you need a different bank setup
set cfg=MMC3_128_128_DPCM.cfg

@REM =========================================
@REM COMPILER DEFINES - configure options for the build
@REM The following options are available. Put a space between each option you want to use
@REM USE_BANKABLE_DPCM - enable if you want to setup the MMC3 to bank $c000 to allow for more than 16k of samples
@REM USE_CHR_A12_INVERT - enable if you want to setup the MMC3 to have fine grain sprite banking. This switches the
@REM                      banking granularity of sprites from 2kb to 1kb.
@REM USE_FAMITONE2 - selects the famitone2 audio engine for music playback
@REM USE_FAMISTUDIO - selects the famistudio audio engine for music playback
set defines=USE_BANKABLE_DPCM USE_CHR_A12_INVERT USE_FAMISTUDIO

@REM =========================================
@REM INTERNAL SETUP - shouldn't need to edit below this line

setlocal EnableDelayedExpansion

for %%s in (%defines%) do (
  set "defines_concatted=!defines_concatted! -D%%s --asm-define %%s"
)

cl65 %defines_concatted% -Oirs -g -C %cfg% -t nes --add-source -Ln %out%\labels.txt -o %out%\%name%.nes --ld-args --dbgfile,%out%\%name%.dbg %srcs%

if %ERRORLEVEL% EQU 0 (
    @REM del *.o Not needed with the switch to CL65
    move /Y labels.txt %out%\
    move /Y %name%.s %out%\
    move /Y %name%.nes %out%\
    echo Build successful!
    @REM pause
) else (
    echo Build failed, check the error messages above for details
    @REM pause
)
