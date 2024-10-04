CXX = g++

CXXFLAGS = -O2 -Wall -g

TARGET=main
SRC= main.cpp parkinglot.cpp vehicle.cpp

OBJ = $(SRC:.cpp=.o)

# Default rule to build the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

# Clean rule to remove the generated files
clean:
	rm -f $(OBJ) $(TARGET)