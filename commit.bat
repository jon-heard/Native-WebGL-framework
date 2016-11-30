echo off
D:
cd D:\_projects\Native-WebGL-framework\Native-WebGL-framework
git stage *
git stage /u
@echo off
set /p id="Enter message: "
git commit -m "%id%"
echo GIT commit DONE