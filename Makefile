BUILD_DIR := ./build

BUILD := $(BUILD_DIR)/machincod
BUILD_LIB := $(BUILD_DIR)/libmachincod.so

SRCS := $(shell find src/ -type f -name "*.c")
SRCS_LIB := $(shell find core/ -type f -name "*.c")
SRCS_LIB_A := $(shell find core/ -type f -name "*.mac")


OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
OBJS_LIB := $(SRCS_LIB:%=$(BUILD_DIR)/%.o)
OBJS_LIB_A := $(SRCS_LIB_A:%=$(BUILD_DIR)/%.o)


HEADERS_DIR := /usr/local/include/machincod

HEADERS := $(shell find core/ -name *.mach)
DEST_HEADERS := $(addprefix $(HEADERS_DIR)/,$(HEADERS))


C_FLAGS := -Iincludes -O3 -Wall -g

LD_FLAGS := -lm # math.h

CC := gcc

all: $(BUILD) $(BUILD_LIB)
	@echo "Done."

compiler: $(BUILD)
	@echo "Done."

$(BUILD): $(OBJS)
	$(CC) $(OBJS) -o $@ $(C_FLAGS) $(LD_FLAGS) 

$(BUILD_DIR)/src/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@ 

core: $(BUILD_LIB)
	@echo "Done."

$(BUILD_LIB): $(OBJS_LIB) $(OBJS_LIB_A)
	$(CC) $(OBJS_LIB) $(OBJS_LIB_A) -o $@ -shared -fno-stack-protector

$(BUILD_DIR)/core/%.c.o: core/%.c
	@mkdir -p $(dir $@)
	$(CC) -Wall -fpic -c $< -o $@

$(BUILD_DIR)/core/%.mac.o: core/%.mac
	@mkdir -p $(dir $@)
	build/machincod -s $< -o $@.s --no-start
	as $@.s -o $@ -msyntax=intel -mnaked-reg -Z

.PHONY: clean install
clean:
	rm -r $(BUILD_DIR)

$(DEST_HEADERS): $(HEADERS)
	@for v in $(HEADERS); do \
		install -D $$v /usr/local/include/machincod/$$v; \
	done

install: $(DEST_HEADERS)
	cp build/libmachincod.so /lib
	cp build/machincod /usr/local/bin/machincod