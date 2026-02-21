CXX=g++
CXX_FLAGS=
CXX_LIBS=-lglfw -lGL

all: game_glfw

game_glfw: game_glfw.cpp
	$(CXX) -o game_glfw game_glfw.cpp $(CXX_FLAGS) $(CXX_LIBS)

clean:
	rm game_glfw