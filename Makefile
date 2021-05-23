TARGET_EXEC = chess

MAKE_RELEASE ?= 0

ifeq ($(MAKE_RELEASE), 0)
	BUILD_DIR = ./build
else
	BUILD_DIR = ./build_release
	TEMP_VAR := $(shell $(RM) -r $(BUILD_DIR))
endif

SRC_DIRS = ./src

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CC = gcc
CPPFLAGS = $(INC_FLAGS) -MMD -MP
CFLAGS = -Wall -Wextra -pedantic

ifeq ($(MAKE_RELEASE), 0)
	CFLAGS := $(CFLAGS) -g
else
	CFLAGS := $(CFLAGS) -O3
endif

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
