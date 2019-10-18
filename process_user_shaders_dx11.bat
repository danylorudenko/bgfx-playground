rmdir /S /Q ..\runtime\shaders
mkdir ..\runtime\shaders\dx11\

for /r %%V in (shaders\vertex\*.*) do ..\..\.build\win64_vs2017\bin\shadercDebug.exe --verbose -i ..\..\src --varyingdef .\shaders\varying.def.sc --platform windows --profile "vs_5_0" --type vertex -O 0 -f "%%V" -o "..\runtime\shaders\dx11\%%~nV.bin"

for /r %%V in (shaders\fragment\*.*) do ..\..\.build\win64_vs2017\bin\shadercDebug.exe --verbose -i ..\..\src --varyingdef .\shaders\varying.def.sc --platform windows --profile "ps_5_0" --type fragment -O 0 -f "%%V" -o "..\runtime\shaders\dx11\%%~nV.bin"

pause