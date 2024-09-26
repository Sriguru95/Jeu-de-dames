# Makefile for the board game project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall

# Executable name
TARGET = board_game

# Source files
SRCS = main.cpp Piece.cpp Elephant.cpp Rhinoceros.cpp Lion.cpp Case.cpp Barriere.cpp Player.cpp Board.cpp 

# Header files
HEADERS = Piece.hpp Elephant.hpp Rhinoceros.hpp Lion.hpp Case.hpp Barriere.hpp Player.hpp Board.hpp 

# Object files
OBJS = $(SRCS:.cpp=.o)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to build object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run rule
run: $(TARGET)
	./$(TARGET)

# Rule to compile and run the program
all: $(TARGET)
	./$(TARGET)

# Clean rule
clean:
	rm -f $(OBJS) $(TARGET)
