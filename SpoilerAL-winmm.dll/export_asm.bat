@echo off

setlocal EnableDelayedExpansion
if not defined inProcessing (
	set inProcessing=1
	if "%~1" == "" (
		set FileName=export.asm
	) else (
		set FileName=%1
	)
	echo Creating %FileName%...
	%0 >%FileName%
	endlocal
	goto :eof
)
echo .386
echo .model flat, c
echo.
echo public ExportAddresses
for /f "tokens=1,2,3 delims=(," %%a in (export.h) do (
	set name=%%b
	if defined name (
		set noname=%%a
		set noname=!noname: =!
		if "!noname!" equ "NAMED" (
			set name=!name: =!
			if defined name (
				set suffix=%%c
				set suffix=!suffix: =!
				echo public _imp__!name!!suffix!
			)
		)
	)
)
echo.
echo .data
echo.
echo align 4
echo ExportAddresses label dword
set len=0
for /f "tokens=2,3 delims=(," %%a in (export.h) do (
	set name=%%a
	set name=!name: =!
	call :strlen i name
	set suffix=%%b
	set suffix=!suffix: =!
	call :strlen j suffix
	set /a i+=!j!
	if !len! lss !i! (
		set len=!i!
	)
)
set pad=
for /l %%i in (1,1,%len%) do (
	set pad= !pad!
)
for /f "tokens=2,3 delims=(," %%a in (export.h) do (
	set name=%%a
	set name=!name: =!
	set suffix=%%b
	set suffix=!suffix: =!
	set name=!name!!suffix!!pad!
	set name=!name:~0,%len%!
	echo _imp__!name! dword ?
)
echo.
echo end
endlocal
goto :eof

:strlen <result> <string>
(
	setlocal EnableDelayedExpansion
	set tmp=!%~2!
	if defined tmp (
		set len=1
		for %%i in (4096 2048 1024 512 256 128 64 32 16 8 4 2 1) do (
			if not "!tmp:~%%i,1!" == "" (
				set /a len+=%%i
				set tmp=!tmp:~%%i!
			)
		)
	) else (
		set len=0
	)
)
(
	endlocal
	set %~1=%len%
	exit /b
)
