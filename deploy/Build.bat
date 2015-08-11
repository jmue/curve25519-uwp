REM MSBuild EXE path
SET MSBuildPath="C:\Program Files (x86)\MSBuild\14.0\Bin\MSBuild.exe"
SET NuGetPath="C:\Program Files (x86)\NuGet\nuget.exe"
set StagingPath=deploy\staging

REM change to the source root directory
pushd ..


REM ======================= clean =======================================

REM ensure any previously created package is deleted
del *.nupkg

REM ensure staging folder is empty
mkdir %StagingPath%
rmdir /s /q %StagingPath%
rmdir /s /q %StagingPath%

REM ======================= build =======================================

REM Build out staging NuGet folder structure
mkdir %StagingPath%
mkdir %StagingPath%\build\uap\native
mkdir %StagingPath%\include
REM mkdir %StagingPath%\lib\uap10.0
mkdir %StagingPath%\runtimes\win10-arm\native
mkdir %StagingPath%\runtimes\win10-x64\native
mkdir %StagingPath%\runtimes\win10-x86\native

REM Stage platform-independent files
copy deploy\nuget\curve25519.targets %StagingPath%\build\uap\native
copy curve25519\Curve25519_Internal.h %StagingPath%\include
copy curve25519\Curve25519Native.h %StagingPath%\include


REM build x86
%MSBuildPath% curve25519\curve25519.vcxproj /ds /fileLogger /property:Configuration=Release /property:Platform=x86
REM stage x86
copy curve25519\Release\curve25519\curve25519.dll %StagingPath%\runtimes\win10-x86\native
copy curve25519\Release\curve25519\curve25519.winmd %StagingPath%\runtimes\win10-x86\native
copy curve25519\Release\curve25519\curve25519.pri %StagingPath%\runtimes\win10-x86\native

REM build x64
%MSBuildPath% curve25519\curve25519.vcxproj /ds /fileLogger /property:Configuration=Release /property:Platform=x64
REM stage x64
copy curve25519\x64\Release\curve25519\curve25519.dll %StagingPath%\runtimes\win10-x64\native
copy curve25519\x64\Release\curve25519\curve25519.winmd %StagingPath%\runtimes\win10-x64\native
copy curve25519\x64\Release\curve25519\curve25519.pri %StagingPath%\runtimes\win10-x64\native

REM build ARM
%MSBuildPath% curve25519\curve25519.vcxproj /ds /fileLogger /property:Configuration=Release /property:Platform=ARM
REM stage ARM
copy curve25519\ARM\Release\curve25519\curve25519.dll %StagingPath%\runtimes\win10-arm\native
copy curve25519\ARM\Release\curve25519\curve25519.winmd %StagingPath%\runtimes\win10-arm\native
copy curve25519\ARM\Release\curve25519\curve25519.pri %StagingPath%\runtimes\win10-arm\native

REM create NuGet package
REM (Note: Expect 6 Assembly outside lib folder warnings, due to WinRT custom packaging technique)
pushd deploy\staging
%NuGetPath% pack ..\nuget\curve25519.nuspec -outputdirectory ..
popd


REM ============================ done ==================================


REM go back to the build dir
popd
