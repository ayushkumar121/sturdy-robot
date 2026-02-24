PROJECT_NAME=game
CXX=g++
CC=gcc
CXX_FLAGS=-Wall -Wno-missing-braces -g -std=c++23 -Ivender/glad/include -Ivender/stb -Ivender/glfw/include
LIB_FREETYPE_FLAGS=$(shell pkg-config --cflags freetype2)
LIB_FREETYPE_LIBS=$(shell pkg-config --libs freetype2)
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    CXX_LIBS=vender/glfw/lib-arm64mac/libglfw3.a -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL
else
    CXX_LIBS=-lglfw -lGL -lm
endif

SRC := main.cpp Basic.cpp Texture.cpp TextureLibrary.cpp Shader.cpp ShaderLibrary.cpp QuadMesh.cpp Renderer.cpp Gui.cpp Font.cpp TextRenderer.cpp

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(SRC) gl.o stb_image.o
	$(CXX) -o $(PROJECT_NAME) $(SRC) gl.o stb_image.o $(CXX_FLAGS) $(CXX_LIBS) $(LIB_FREETYPE_FLAGS) $(LIB_FREETYPE_LIBS)

gl.o: vender/glad/src/gl.c
	$(CC) -c -o gl.o vender/glad/src/gl.c -Ivender/glad/include 

stb_image.o: vender/stb/stb_image.c
	$(CC) -c -o stb_image.o vender/stb/stb_image.c -Ivender/stb

clean:
	rm -f $(PROJECT_NAME) gl.o stb_image.o