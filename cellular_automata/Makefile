CXX := g++
CXXFLAGS := -Wall -std=c++17 -I/path/to/SFML/include
LDFLAGS := -L/path/to/SFML/lib
LDLIBS := -lsfml-graphics -lsfml-window -lsfml-system

TARGET := cell_program
OBJECTS := main.o CellGrid.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS) $(LDLIBS)

main.o: main.cpp CellGrid.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

CellGrid.o: CellGrid.cpp CellGrid.hpp
	$(CXX) $(CXXFLAGS) -c CellGrid.cpp

.PHONY: clan run
clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
