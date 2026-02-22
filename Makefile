PROJECT_NAME=game
CXX=g++
CXX_FLAGS=-Wall -std=c++23 -Ivender/glad/include -Ivender/stb
CXX_LIBS=-lglfw -lm -lGL

SRC := main.cpp Shader.cpp Basic.cpp Texture.cpp ShaderLibrary.cpp vender/glad/src/gl.c

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(SRC)
	$(CXX) -o $(PROJECT_NAME) $(SRC) $(CXX_FLAGS) $(CXX_LIBS)

clean:
	rm -f $(PROJECT_NAME)