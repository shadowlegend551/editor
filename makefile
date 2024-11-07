CC := gcc
CFLAGS := -lm

TARGET_FILE := bin/out
INCLUDE_DIR := include
SRC_DIR := src
BUILD_DIR := build
BIN_DIR := bin


SRC_FILES := $(wildcard $(SCR_DIR)/*.c)
OBJ_FILES := $(patsubst $(BUILD_DIR)/%.o $(SRC_FILES))

# Build from source.
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -I $(INCLUDE_DIR) -c $< -o $@

# Link object files.
$(TARGET_FILE): $(OBJ_FILES)
	$(CC) $^ -o $(TARGET_FILE)

.PHONY: build
