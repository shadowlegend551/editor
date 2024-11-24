CC := gcc
CFLAGS := -lm

SRC_DIR := src
BUILD_DIR := build
TARGET_FILE := bin/main

SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC_FILES))


$(TARGET_FILE): $(OBJ_FILES)
	$(CC) -o $@ $^ $(CFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

clean:
	rm $(OBJ_FILES) $(TARGET_FILE)

.PHONY:
	clean

