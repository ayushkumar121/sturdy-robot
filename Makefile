PROJECT_NAME=game
CXX=g++
CXX_FLAGS=-Wall -std=c++17 -Ivender/glad/include
CXX_LIBS=-lglfw -lGL

SRC := $(wildcard *.cpp)  vender/glad/src/gl.c

all: $(PROJECT_NAME)

$(PROJECT_NAME): $(SRC)
	$(CXX) -o $(PROJECT_NAME) $(SRC) $(CXX_FLAGS) $(CXX_LIBS)

clean:
	rm -f $(PROJECT_NAME)