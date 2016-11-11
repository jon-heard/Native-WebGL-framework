all: pc web



pc: bin/app.exe

bin/app.exe: obj/pc/main.o obj/pc/platform.o obj/pc/shaders.o obj/pc/errorHandling.o
	g++ obj/pc/main.o obj/pc/platform.o obj/pc/shaders.o obj/pc/errorHandling.o -o bin/app.exe -L libs/glfw/lib -L libs/glew/lib -L libs/soil/lib -lglew32s -lglfw3 -lsoil -lopengl32 -lgdi32 -static-libgcc -static-libstdc++ -static

obj/pc/main.o: src/main.cpp
	g++ -c src/main.cpp -o obj/pc/main.o

obj/pc/platform.o: src/platform.cpp
	g++ -c src/platform.cpp -o obj/pc/platform.o -I libs/glfw/include -I libs/glew/include -I libs/soil/include

obj/pc/shaders.o: src/shaders.cpp
	g++ -c src/shaders.cpp -o obj/pc/shaders.o -I libs/glew/include

obj/pc/errorHandling.o: src/errorHandling.cpp
	g++ -c src/errorHandling.cpp -o obj/pc/errorHandling.o



web_notWorking: bin/app.js

bin/app.js: obj/web/main.bc obj/web/platform.bc obj/web/shaders.bc obj/web/errorHandling.bc
	call em++ obj/web/main.bc obj/web/platform.bc obj/web/shaders.bc obj/web/errorHandling.bc -o bin/app.js -lglew -s USE_GLFW=3

obj/web/main.bc: src/main.cpp
	call em++ -c src/main.cpp -o obj/web/main.bc -I libs/glfw/include -I libs/glew/include -s USE_GLFW=3

obj/web/platform.bc: src/platform.cpp
	call em++ -c src/platform.cpp -o obj/web/platform.bc -I libs/glfw/include -I libs/glew/include -s USE_GLFW=3

obj/web/shaders.bc: src/shaders.cpp
	call em++ -c src/shaders.cpp -o obj/web/shaders.bc -I libs/glew/include -s USE_GLFW=3

obj/web/errorHandling.bc: src/errorHandling.cpp
	call em++ -c src/errorHandling.cpp -o obj/web/errorHandling.bc -s USE_GLFW=3

web:
	call em++ src/main.cpp src/platform.cpp src/shaders.cpp src/errorHandling.cpp -o bin/app.js -s USE_GLFW=3


clean:
	del /F /Q obj\pc\*
	del /F /Q obj\web\*
	del /F /Q bin\*
