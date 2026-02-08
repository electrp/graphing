@echo off
set "dest=.\submission"

if not exist "%dest%" (
	mkdir "%dest%"
)

xcopy ".\src\*" "%dest%\src\" /E /H /C /I /Y /S
xcopy ".\scripts\*" "%dest%\scripts\" /E /H /C /I /Y /S
xcopy ".\shaders\*" "%dest%\shaders\" /E /H /C /I /Y /S
xcopy ".\extern\*" "%dest%\extern\" /E /H /C /I /Y /S
xcopy ".\CMakeLists.txt" "%dest%\" /C /Y
xcopy ".\generate_project.bat" "%dest%\" /C /Y
xcopy ".\README.md" "%dest%\" /C /Y
