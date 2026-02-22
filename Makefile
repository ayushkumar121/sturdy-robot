PROJECT_NAME=game
CXX=g++
CXX_FLAGS=-Wall -Wno-missing-braces -g -std=c++23 -Ivender/glad/include -Ivender/stb -Ivender/glfw/include

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    CXX_LIBS=vender/glfw/lib-arm64mac/libglfw3.a -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL
else
    CXX_LIBS=-lglfw -lGL -lX11 -ldl -lpthread -lm
endif

SRC := main.cpp Basic.cpp Texture.cpp TextureLibrary.cpp Shader.cpp ShaderLibrary.cpp Quad.cpp QuadMesh.cpp Renderer.cpp vender/glad/src/gl.c

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(SRC)
	$(CXX) -o $(PROJECT_NAME) $(SRC) $(CXX_FLAGS) $(CXX_LIBS)

clean:
	rm -f $(PROJECT_NAME)