CC := g++
CFLAGS += -g -O2 -Wall
LDFLAGS += -L/usr/include/readline -lreadline

SRC_DIR := src
OBJ_DIR = ./obj_dir

CSRCS := $(shell find $(abspath $(SRC_DIR)) \
		-path "$(abspath $(SRC_DIR))/backup" -prune \
		-o -name "*.c" -o -name "*.cc" -o -name "*.cpp" -print)

#LLVM_CFLAGS := $(shell llvm-config --cxxflags)
#LLVM_LDFLAGS := $(shell llvm-config --ldflags) $(shell llvm-config --libs) $(shell llvm-config --system-libs)

# 转换源文件列表为对象文件列表
OBJS := $(CSRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
OBJS := $(OBJS:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)
OBJS := $(OBJS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# 目标文件名
TARGET := simulator

# 默认目标
all: $(TARGET)

# 链接对象文件生成最终可执行文件
$(TARGET): $(OBJS)
	mkdir -p $(OBJ_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# 从.c, .cc, .cpp文件生成.o文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# 清除构建产物的命令
clean:
	@rm -rf $(OBJ_DIR) $(TARGET)

# 声明伪目标
.PHONY: all clean