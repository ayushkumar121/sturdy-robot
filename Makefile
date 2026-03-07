PROJECT_NAME=game
CXX=g++
CC=gcc
CXX_FLAGS=-Wall -Wno-missing-braces -Wno-deprecated-declarations -g -std=c++23 -Ivender/glad/include -Ivender/stb -Ivender/glfw/include $(shell pkg-config --cflags freetype2)

UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
    CXX_LIBS=-Lvender/glfw/lib-arm64mac -lglfw3 -framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL $(shell pkg-config --libs freetype2)
else
    CXX_LIBS=-lglfw -lGL -lm $(shell pkg-config --libs freetype2)
endif

# Source files
SRC :=main.cpp Basic.cpp Texture.cpp DummyTexture.cpp TextureLibrary.cpp Shader.cpp ShaderLibrary.cpp \
QuadMesh.cpp Renderer.cpp Gui.cpp Font.cpp FontLibrary.cpp TextRenderer.cpp \
StoryEngine.cpp TaskSystem.cpp Game.cpp EmailSystem.cpp Camera.cpp
OBJ := $(SRC:.cpp=.o)

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(OBJ) gl.o stb_image.o
	$(CXX) -o $@ $^ $(CXX_LIBS)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

gl.o: vender/glad/src/gl.c
	$(CC) -c -o gl.o vender/glad/src/gl.c -Ivender/glad/include 

stb_image.o: vender/stb/stb_image.c
	$(CC) -c -o stb_image.o vender/stb/stb_image.c -Ivender/stb

$(PROJECT_NAME).app: $(PROJECT_NAME)
	rm -rf $@
	mkdir -p $@/Contents/MacOS
	mkdir -p $@/Contents/Resources
	cp $(PROJECT_NAME) $@/Contents/MacOS/
	cp -R assets $@/Contents/Resources/
	cp Info.plist $@/Contents

clean:
	rm -f $(PROJECT_NAME) $(OBJ) gl.o stb_image.o
	rm -rf $(PROJECT_NAME).app
