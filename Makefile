# 定义编译器
CC=clang

# 定义编译选项，-g 用于调试，-Wall 显示所有警告
CFLAGS=-g -Wall

# 定义目标文件和源文件的目录
BIN_DIR=bin
SRC_DIR=src
INCLUDE_DIR=src

# 定义目标文件和源文件
TARGET=scanner
SOURCES=$(SRC_DIR)/main.c $(SRC_DIR)/scanner.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=$(BIN_DIR)/$(TARGET)

# 默认目标
all: $(EXECUTABLE)

# 链接目标
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# 编译源文件
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# 清理编译生成的文件
clean:
	rm -f $(BIN_DIR)/$(TARGET) $(OBJECTS)

# 包含依赖文件
-include $(OBJECTS:.o=.d)

# 伪目标，用于依赖关系
.PHONY: all clean