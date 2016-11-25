cls
call em++ -o web/app.js src/main.cpp src/platform/draw.cpp src/platform/DrawCache.cpp src/platform/errorHandling.cpp src/platform/input.cpp src/platform/platform.cpp src/platform/Shader.cpp libs/soil/image_DXT.c libs/soil/image_helper.c libs/soil/SOIL.c libs/soil/stb_image_aug.c -I libs/soil -s USE_GLFW=3 --preload-file media
