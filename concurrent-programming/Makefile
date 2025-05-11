CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
LDFLAGS = -lpthread

BUILD_DIR = ./build

TARGET = ContarPalabras

# Nombres de los objetivos de prueba
TEST_TARGETS = UnitTests1 UnitTests2 UnitTests3 UnitTests4
#TEST_TARGETS =  UnitTests4
# Archivos de objetos que necesitas para tu proyecto
OBJECTS = HashMapConcurrente.o CargarArchivos.o

all: build $(BUILD_DIR)/$(TARGET)

# Regla para compilar y ejecutar todas las pruebas
test: build $(patsubst %, $(BUILD_DIR)/%, $(TEST_TARGETS))
	@for target in $(TEST_TARGETS); do \
		$(BUILD_DIR)/$$target; \
	done

# Regla para compilar y ejecutar solo UnitTests1
test1: build $(BUILD_DIR)/UnitTests1
	$(BUILD_DIR)/UnitTests1

# Regla para compilar y ejecutar solo UnitTests2
test2: build $(BUILD_DIR)/UnitTests2
	$(BUILD_DIR)/UnitTests2

# Regla para compilar y ejecutar solo UnitTests3
test3: build $(BUILD_DIR)/UnitTests3
	$(BUILD_DIR)/UnitTests3

# Regla para compilar y ejecutar solo UnitTests4
test4: build $(BUILD_DIR)/UnitTests4
	$(BUILD_DIR)/UnitTests4

# Regla para compilar cada objetivo de prueba
$(BUILD_DIR)/%: src/%.cpp $(OBJECTS:%=$(BUILD_DIR)/%)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: src/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDFLAGS)

.PHONY: all test build clean debug

build:
	@mkdir -p $(BUILD_DIR)

debug: CXXFLAGS += -DDEBUG -g
debug: all

clean:
	-@rm -rvf $(BUILD_DIR)/*
