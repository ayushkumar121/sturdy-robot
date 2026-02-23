PROJECT_NAME=game
CXX=g++
CC=gcc
CXX_FLAGS=-Wall -Wno-missing-braces -g -std=c++23 -Ivender/glad/include -Ivender/stb -Ivender/glfw/include

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    CXX_LIBS=vender/glfw/lib-arm64mac/libglfw3.a -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL
else
    CXX_LIBS=-lglfw -lGL -lm
endif

SRC := main.cpp Basic.cpp Texture.cpp TextureLibrary.cpp Shader.cpp ShaderLibrary.cpp Quad.cpp QuadMesh.cpp Renderer.cpp Gui.cpp

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(SRC) gl.o
	$(CXX) -o $(PROJECT_NAME) $(SRC) gl.o $(CXX_FLAGS) $(CXX_LIBS)

gl.o: vender/glad/src/gl.c
	$(CC) -c -o gl.o vender/glad/src/gl.c -Ivender/glad/include 

clean:
	rm -f $(PROJECT_NAME)