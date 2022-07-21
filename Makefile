TARGET_EXEC = chess

DEBUG ?= 0

BASE_BUILD_DIR = ./build

ifeq ($(DEBUG), 0)
	BUILD_DIR = $(BASE_BUILD_DIR)/release
else
	BUILD_DIR = $(BASE_BUILD_DIR)/debug
endif

SRC_DIRS = ./src

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

CC = gcc
CPPFLAGS = $(INC_FLAGS) -MMD -MP
CFLAGS = -Wall -Wextra -pedantic -Werror

LDFLAGS = -pthread

ifeq ($(DEBUG), 0)
	CFLAGS := $(CFLAGS) -O3
else
	CFLAGS := $(CFLAGS) -g -DDEBUG
endif

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@


.PHONY: clean

clean:
	$(RM) -r $(BASE_BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
