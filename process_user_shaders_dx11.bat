rmdir /S /Q .\workdir\shaders\
mkdir .\workdir\shaders\

setlocal EnableDelayedExpansion

for /r %%V in (.\shaders\vertex\*.*) do ( 
	.\tools\shaderc\shaderc.exe --verbose -i .\submodules\bgfx\src --varyingdef .\shaders\varying.def.sc --platform windows --profile "vs_5_0" --type vertex -O 0 -f "%%V" -o ".\workdir\shaders\%%~nV.bin"  
	if !ERRORLEVEL! NEQ 0 GOTO end 
)

for /r %%V in (.\shaders\fragment\*.*) do ( 
	.\tools\shaderc\shaderc.exe --verbose -i .\submodules\bgfx\src --varyingdef .\shaders\varying.def.sc --platform windows --profile "ps_5_0" --type fragment -O 0 -f "%%V" -o ".\workdir\shaders\%%~nV.bin" 
	if !ERRORLEVEL! NEQ 0 GOTO end
)

endlocal

exit

:end
pause