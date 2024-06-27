# Указать компилятор
CC = gcc

# Папки проекта
SRC_DIR = src
INCLUDE_DIR = include
LIB_SRC_DIR = $(SRC_DIR)/lib
BIN_DIR = bin
BUILD_DIR = build

# Названия выходных файлов
LIBRARY = $(BIN_DIR)/libcalc.so.1.0.0
LIBRARY_LINK = $(BIN_DIR)/libcalc.so
EXECUTABLE = $(BIN_DIR)/calculator

# Исходные файлы и объектные файлы
LIB_SRC = $(wildcard $(LIB_SRC_DIR)/*.c)
SRC = $(wildcard $(SRC_DIR)/*.c)
SRC := $(filter-out $(LIB_SRC), $(SRC))  # Убрать файлы библиотеки
LIB_OBJ = $(patsubst $(LIB_SRC_DIR)/%.c, $(BUILD_DIR)/lib_%.o, $(LIB_SRC))
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Правило по умолчанию
all: $(LIBRARY) $(LIBRARY_LINK) $(EXECUTABLE)

# Сборка динамической библиотеки
$(LIBRARY): $(LIB_OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) -shared -o $@ $^ -I$(INCLUDE_DIR) -fPIC

# Создание символической ссылки на библиотеку
$(LIBRARY_LINK): $(LIBRARY)
	ln -sf $(notdir $(LIBRARY)) $@

# Сборка исполняемого файла
$(EXECUTABLE): $(OBJ) $(LIBRARY)
	mkdir -p $(BIN_DIR)
	$(CC) -o $@ $^ -I$(INCLUDE_DIR) -L$(BIN_DIR) -lcalc -ldl -Wl,-rpath,.

# Компиляция исходных файлов библиотеки
$(BUILD_DIR)/lib_%.o: $(LIB_SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@ -I$(INCLUDE_DIR) -fPIC

# Компиляция исходных файлов проекта
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) -c $< -o $@ -I$(INCLUDE_DIR)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

.PHONY: all clean
