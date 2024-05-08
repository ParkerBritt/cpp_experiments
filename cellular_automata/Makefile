CXX := g++
CXXFLAGS := -Wall -std=c++17 -I/path/to/SFML/include
LDFLAGS := -L/path/to/SFML/lib
LDLIBS := -lsfml-graphics -lsfml-window -lsfml-system

TARGET := cell_program
SRC_DIR := src
BUILD_DIR := build
OBJECTS := $(BUILD_DIR)/main.o $(BUILD_DIR)/CellGrid.o

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS) $(LDLIBS)

# build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: clean run
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

run: $(TARGET)
	./$(TARGET)
