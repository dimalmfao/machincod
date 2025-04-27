BUILD_DIR := ./build

BUILD := $(BUILD_DIR)/machincod
BUILD_LIB := $(BUILD_DIR)/libmachincod.so

SRCS := $(shell find src/ -name '*.c')
SRCS_LIB := $(shell find core/ -name '*.c')
SRCS_LIB_A := $(shell find core/ -name '*.mac')

OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
OBJS_LIB := $(SRCS_LIB:%=$(BUILD_DIR)/%.o)
OBJS_LIB_A := $(SRCS_LIB_A:%=$(BUILD_DIR)/%.o)

HEADERS_DIR := /usr/local/include/machincod
HEADERS := $(shell find core/ -name '*.mach')
DEST_HEADERS := $(addprefix $(HEADERS_DIR)/,$(HEADERS))

C_FLAGS := -Iincludes -O3 -Wall
LD_FLAGS := -lm # math.h

CC := gcc

# Targets to build
all: $(BUILD) $(BUILD_LIB)
	@echo "Done."

# Main executable
$(BUILD): $(OBJS)
	@echo "Linking object files: $(OBJS)"
	$(CC) $(OBJS) -o $@ $(C_FLAGS) $(LD_FLAGS)

# Object files for the main executable
$(BUILD_DIR)/src/%.c.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@

# Shared library
$(BUILD_LIB): $(OBJS_LIB) $(OBJS_LIB_A)
	$(CC) $(OBJS_LIB) $(OBJS_LIB_A) -o $@ -shared -fno-stack-protector -mincoming-stack-boundary=3

# Object files for the core source files
$(BUILD_DIR)/core/%.c.o: core/%.c
	@mkdir -p $(dir $@)
	$(CC) -Wall -fpic -c $< -o $@ -mincoming-stack-boundary=3

# Object files for the core .mac files
$(BUILD_DIR)/core/%.mac.o: core/%.mac
	@mkdir -p $(dir $@)
	build/machincod -s $< -o $@.s --no-start --assembly
	as $@.s -o $@ -msyntax=intel -mnaked-reg

# Clean target
.PHONY: clean install uninstall
clean:
	rm -r $(BUILD_DIR)

# Installing headers
$(DEST_HEADERS): $(HEADERS)
	@for v in $(HEADERS); do \
		install -D $$v /usr/local/include/machincod/$$v; \
	done

# Install the compiled library and executable
install: $(DEST_HEADERS)
	cp build/libmachincod.so /lib
	cp build/machincod /usr/local/bin/machincod

	@if test -f /lib/ld-linux-x86-64.so.2; then \
		ln -sf /lib/ld-linux-x86-64.so.2 /lib/ld64.so.1; \
	else \
		if test -f /lib/x86_64-linux-gnu/ld-linux-x86-64.so.2; then \
			ln -sf /lib/x86_64-linux-gnu/ld-linux-x86-64.so.2 /lib/ld64.so.1; \
		else \
			if test -f /lib64/ld-linux-x86-64.so.2; then \
				ln -sf /lib64/ld-linux-x86-64.so.2 /lib/ld64.so.1; \
			else \
				echo "Did not find any matching loader..."; \
			fi; \
		fi; \
	fi; \

# Uninstall target
uninstall:
	rm -r /lib/libmachincod.so
	rm -rf /usr/local/include/machincod/
	rm /usr/local/bin/machincod
