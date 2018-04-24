# Detect the number of CPU cores and do parallel build automatically.
NUM_CORES := $(shell grep -c ^processor /proc/cpuinfo)
MAKEFLAGS += --jobs=$(NUM_CORES) --max-load=$(NUM_CORES)

PROG := static_test

INC_DIR    := inc
SRC_DIR    := test
VENDOR_DIR := vendor
BIN_DIR    := bin
INT_DIR    := $(BIN_DIR)/intermediates
DOC_DIR    := doc

EXE := $(BIN_DIR)/$(PROG)

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)%,$(INT_DIR)%.o,$(SRCS))
DEPS := $(patsubst $(SRC_DIR)%,$(INT_DIR)%.d,$(SRCS))

INCLUDE_PATH := -I $(INC_DIR)
INCLUDE_PATH += -I $(VENDOR_DIR)

CXX      := g++

CPPFLAGS += -std=c++98
CPPFLAGS += -Wall -Wextra
CPPFLAGS += -g -O0
CPPFLAGS += $(INCLUDE_PATH)

LDFLAGS += -Xlinker -Map=$(INT_DIR)/linker.map

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

.PHONY: all clean docs fmt test

test: $(EXE)
	@echo 'Running $^ ...'
	$(QUIET)$(EXE) --use-colour yes --order rand --rng-seed time

$(EXE): $(OBJS)
	@echo 'Linking $@ ...'
	$(QUIET)$(CXX) $(LDFLAGS) -o $@ $^

$(INT_DIR):
	$(QUIET)mkdir -p $(INT_DIR)

$(INT_DIR)/%.o: $(SRC_DIR)/% | $(INT_DIR)
	@echo 'Compiling $< ...'
	$(QUIET)$(CXX) $(CPPFLAGS) -MMD -c $< -o $@

-include $(DEPS)

fmt:
	@echo 'Formatting library headers ...'
	$(QUIET)find ./inc \( -iname '*.h' -o -iname '*.c' -o -iname '*.cpp' \) -exec clang-format -i -style=file -fallback-style=none {} \;
	@echo 'Formatting unit tests ...'
	$(QUIET)find ./test \( -iname '*.h' -o -iname '*.c' -o -iname '*.cpp' \) -exec clang-format -i -style=file -fallback-style=none {} \;

$(DOC_DIR):
	$(QUIET)mkdir -p $(DOC_DIR)

docs: | $(DOC_DIR)
	$(QUIET)rm -rf $(DOC_DIR)/html
	@echo 'Generating documentation ...'
	$(QUIET)doxygen doc/Doxyfile

clean:
	@echo 'Cleaning ...'
	$(QUIET)rm -rf $(BIN_DIR)/*
