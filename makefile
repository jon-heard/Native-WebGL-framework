all: pc web



pc: bin/app.exe

bin/app.exe: obj/main.o obj/shaders.o obj/errorHandling.o
	g++ obj/main.o obj/shaders.o obj/errorHandling.o -o bin/app.exe -L libs/glfw/lib -L libs/glew/lib -lglew32s -lglfw3 -lopengl32 -lgdi32

obj/main.o: src/main.cpp
	g++ -c src/main.cpp -o obj/main.o -I libs/glfw/include -I libs/glm/include -I libs/glew/include -L libs/glfw/lib -L libs/glew/lib -lglew32s -lglfw3 -lopengl32 -lgdi32

obj/shaders.o: src/shaders.cpp
	g++ -c src/shaders.cpp -o obj/shaders.o -I libs/glfw/include -I libs/glm/include -I libs/glew/include -L libs/glfw/lib -L libs/glew/lib -lglew32s -lglfw3 -lopengl32 -lgdi32

obj/errorHandling.o: src/errorHandling.cpp
	g++ -c src/errorHandling.cpp -o obj/errorHandling.o -I libs/glm/include -I libs/glfw/include -I libs/glew/include -L libs/glfw/lib -L libs/glew/lib -lglew32s -lglfw3 -lopengl32 -lgdi32



web: bin/app.js

bin/app.js: src/main.cpp src/shaders.cpp src/errorHandling.cpp
	emcc src/main.cpp src/shaders.cpp src/errorHandling.cpp -o bin/app.js -s USE_GLFW=3



clean:
	del /F /Q obj\*
	del /F /Q bin\*
