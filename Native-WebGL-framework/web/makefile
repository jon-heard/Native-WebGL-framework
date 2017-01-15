CPP_NONPLATFORM_FILES := $(wildcard ../src/nonplatform/*.cpp)
CPP_PLATFORM_FILES := $(wildcard ../src/platform/*.cpp)
CPP_MAIN := $(wildcard ../src/main.cpp)
CPP_FILES_LUA := $(wildcard ../libs/lua/*.c)
CPP_FILES_SOIL := $(wildcard ../libs/soil/*.c)


web:
	call emcc $(CPP_NONPLATFORM_FILES) $(CPP_PLATFORM_FILES) $(CPP_MAIN) $(CPP_FILES_LUA) $(CPP_FILES_SOIL) ../libs/freetype/lib/freetype.bc -I ../libs/soil -I ../libs/freetype/include -o app.js -s USE_GLFW=3 -s DEMANGLE_SUPPORT=1 -s ALLOW_MEMORY_GROWTH=1 --preload-file ../media -O2


clean:
	del /F /Q take3.js
