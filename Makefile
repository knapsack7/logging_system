# Makefile for Multithreaded Logging System

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

# Target executable
TARGET = logger_app

# Source files
SRCS = main.cpp logger.cpp
OBJS = $(SRCS:.cpp=.o)

# Include directories (if any)
INCLUDES = -I.

# Build the target
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)
