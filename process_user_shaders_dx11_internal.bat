rmdir /S /Q ..\workdir\shaders\
mkdir ..\workdir\shaders\

setlocal EnableDelayedExpansion

for /r %%V in (..\shaders\vertex\*.*) do ( 
	..\tools\shaderc\shaderc.exe --verbose -i ..\submodules\bgfx\src --varyingdef ..\shaders\varying.def.sc --platform windows --profile "vs_5_0" --type vertex -O 0 -f "%%V" -o ".\shaders\%%~nV.bin"  
	if !ERRORLEVEL! NEQ 0 EXIT /B 1 
)

for /r %%V in (..\shaders\fragment\*.*) do ( 
	..\tools\shaderc\shaderc.exe --verbose -i ..\submodules\bgfx\src --varyingdef ..\shaders\varying.def.sc --platform windows --profile "ps_5_0" --type fragment -O 0 -f "%%V" -o ".\shaders\%%~nV.bin" 
	if !ERRORLEVEL! NEQ 0 EXIT /B 1
)

endlocal
