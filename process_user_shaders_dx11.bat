rmdir /S /Q .\shaders\bin
mkdir .\shaders\bin\

for /r %%V in (shaders\vertex\*.*) do .\tools\shaderc\shaderc.exe --verbose -i .\submodules\bgfx\src --varyingdef .\shaders\varying.def.sc --platform windows --profile "vs_5_0" --type vertex -O 0 -f "%%V" -o ".\shaders\bin\%%~nV.bin"

for /r %%V in (shaders\fragment\*.*) do .\tools\shaderc\shaderc.exe --verbose -i .\submodules\bgfx\src --varyingdef .\shaders\varying.def.sc --platform windows --profile "ps_5_0" --type fragment -O 0 -f "%%V" -o ".\shaders\bin\%%~nV.bin"

pause