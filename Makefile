PROJECT_NAME=game
CXX=g++
CXX_FLAGS=-Wall -g -std=c++23 -Ivender/glad/include -Ivender/stb
CXX_LIBS=-lglfw -lm -lGL

SRC := main.cpp Basic.cpp Texture.cpp TextureLibrary.cpp Shader.cpp ShaderLibrary.cpp Quad.cpp QuadMesh.cpp Renderer.cpp vender/glad/src/gl.c

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(SRC)
	$(CXX) -o $(PROJECT_NAME) $(SRC) $(CXX_FLAGS) $(CXX_LIBS)

clean:
	rm -f $(PROJECT_NAME)