# Detect the number of CPU cores and do parallel build automatically.
NUM_CORES := $(shell grep -c ^processor /proc/cpuinfo)
MAKEFLAGS += --jobs=$(NUM_CORES) --max-load=$(NUM_CORES)

PROG := static_test

SRC_DIR := test
BIN_DIR := bin
INT_DIR := $(BIN_DIR)/intermediates
DOC_DIR := doc

EXE := $(BIN_DIR)/$(PROG)

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)%,$(INT_DIR)%.o,$(SRCS))
DEPS := $(patsubst $(SRC_DIR)%,$(INT_DIR)%.d,$(SRCS))

INCLUDE_PATH := -I inc
INCLUDE_PATH += -I vendor

CXX      := g++

CPPFLAGS += -std=c++98
CPPFLAGS += -Wall -Wextra -Werror
CPPFLAGS += -g -O0
CPPFLAGS += $(INCLUDE_PATH)

# Allow user to specify a sanitizer.
ifeq ($(SANITIZER), asan)
    CPPFLAGS += -fsanitize=address
    LDFLAGS  += -fsanitize=address
endif
ifeq ($(SANITIZER), ubsan)
    CPPFLAGS += -fsanitize=undefined
    LDFLAGS  += -fsanitize=undefined
endif

# Turn on command output if VERBOSE is set to 1.
VERBOSE ?= 0
ifeq ($(VERBOSE),1)
    QUIET :=
else
    QUIET := @
endif

.PHONY: all docs test clean

all: docs

$(DOC_DIR):
	$(QUIET)mkdir -p $(DOC_DIR)

docs: | $(DOC_DIR)
	$(QUIET)rm -rf $(DOC_DIR)/*
	$(QUIET)doxygen

test: $(EXE)
	$(QUIET)$(EXE) --use-colour yes --order rand --rng-seed time

$(EXE): $(OBJS)
	$(QUIET)echo 'Linking $@ ...'
	$(QUIET)$(CXX) $(LDFLAGS) -o $@ $^

$(INT_DIR):
	$(QUIET)mkdir -p $(INT_DIR)

$(INT_DIR)/%.o: $(SRC_DIR)/% | $(INT_DIR)
	$(QUIET)echo 'Compiling $< ...'
	$(QUIET)$(CXX) $(CPPFLAGS) -MMD -c $< -o $@

-include $(DEPS)

clean:
	$(QUIET)echo 'Cleaning ...'
	$(QUIET)rm -rf $(BIN_DIR)/*
