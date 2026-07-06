CXX = g++

CXXFLAGS = $(shell sdl2-config --cflags) -std=c++20 -Wall -Wextra

LDFLAGS = $(shell sdl2-config --libs)

TARGET = doom

SRC = main.cpp

$(TARGET): $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) -o $(TARGET)

clean:
	rm -f $(TARGET)