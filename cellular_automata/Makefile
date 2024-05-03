CXX := g++

TARGET := cell_program
SOURCES := main.cpp
CXX_FLAGS := -c

$(TARGET): main.o 
	$(CXX) main.o -o $(TARGET) -lsfml-graphics -lsfml-window -lsfml-system
main.o: main.cpp
	$(CXX) $(CXX_FLAGS) $(SOURCES)
run: $(TARGET)
	./$(TARGET)
clean:
	rm -f main.o $(TARGET)
