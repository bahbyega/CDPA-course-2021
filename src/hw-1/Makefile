CC		= gcc
CFLAGS	= -Wall -Wextra -std=c99 -fopenmp -lOpenCL `pkg-config --cflags gtk+-3.0`
LDFLAGS	= -O3 -Os -L/usr/lib/x86_64-linux-gnu
LIBS	= -lm `pkg-config --libs gtk+-3.0`

BUILD_DIR = build
OBJ_DIR = obj
GUI_COMP_DIR = gui_components
TARGET  = out

DIR_GUARD = @mkdir -p $(BUILD_DIR)/$(OBJ_DIR)/$(GUI_COMP_DIR)

SOURCES := $(shell find . -name '*.c')
OBJECTS := $(patsubst %.c,$(BUILD_DIR)/$(OBJ_DIR)/%.o,$(SOURCES))
DEPS=$(OBJECTS:.o=.d)

# Compile
$(BUILD_DIR)/$(OBJ_DIR)/%.o: %.c
	$(DIR_GUARD)
	@echo "Compiling $^:"
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/$(OBJ_DIR)/$(GUI_COMP_DIR)/%.o: %.c
	$(DIR_GUARD)
	@echo "Compiling $^:"
	$(CC) -c $< -o $@ $(CFLAGS)

# Link
$(BUILD_DIR)/$(TARGET): $(OBJECTS)
	$(CC) -MMD -o $@ $^ $(CFLAGS) $(LDFLAGS) $(LIBS)
	@echo "Build successful!"

-include $(DEPS)

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)/$(TARGET) $(OBJECTS) $(DEPS)
