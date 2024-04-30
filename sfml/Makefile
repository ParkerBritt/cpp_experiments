CXX := g++

TARGET := sfml-app
SOURCES := main.cpp
GXX_FLAGS := -c

sfml-app: main.o 
	$(CXX) main.o -o $(TARGET) -lsfml-graphics -lsfml-window -lsfml-system
main.o: main.cpp
	$(CXX) $(GXX_FLAGS) $(SOURCES)

clean:
	rm -f main.o $(TARGET)
