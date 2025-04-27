BUILD_DIR := build
BUILD := $(BUILD_DIR)/compiler
BUILD_LIB := $(BUILD_DIR)/libmachincod.so

SRCS := $(shell find src -name '*.c')
SRCS_LIB := $(shell find core -name '*.c')

OBJS := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS))
OBJS_LIB := $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRCS_LIB))

C_FLAGS := -Iincludes -O3 -Wall -g
LD_FLAGS := -lm
CC := gcc

# Create necessary build directories automatically
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@

# Targets
all: $(BUILD) $(BUILD_LIB)
	@echo "✅ Build complete."

compiler: $(BUILD)
	@echo "✅ Compiler built."

core: $(BUILD_LIB)
	@echo "✅ Core library built."

$(BUILD): $(OBJS)
	@echo "🔨 Linking compiler"
	$(CC) $(OBJS) -o $@ $(C_FLAGS) $(LD_FLAGS)

$(BUILD_LIB): $(OBJS_LIB)
	@echo "🔨 Linking shared library"
	$(CC) $(OBJS_LIB) -shared -fno-stack-protector -o $@

.PHONY: clean install

clean:
	rm -rf $(BUILD_DIR)

install:
	cp $(BUILD_LIB) /lib
	cp $(BUILD) /usr/local/bin/arobase
