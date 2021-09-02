CXX ?= clang++
CXXWARN ?= -Werror -Wall -Wextra -Wno-unused-parameter -pedantic

CXXSTD ?= c++17
SRC=src/main.cpp

BUILD_DIR=build
TARGET=pvc

# Testing
TEST_FILE=test_cases
TEST_DIR=test
TEST_TARGET=$(basename $(subst $(TEST_DIR),$(BUILD_DIR),$(wildcard $(TEST_DIR)/*.cpp)))

# Libraries to include and link
INC=-Isrc/
LIBS=$(LDLIBS)

# Flags
debug ?= yes
sanitizer ?= no

# Debug flags
ifeq ($(debug),no)
	CXXFLAGS+=-O3 -march=native -flto -DNDEBUG -s
else ifeq ($(debug),yes)
	CXXFLAGS+=-Og -g -march=native
else
$(error debug should be either yes or no)
endif

# Sanitizer flags
ifeq ($(sanitizer),yes)
	CXXFLAGS+=-fsanitize=address,undefined,leak
else ifeq ($(sanitizer),no)

else
$(error sanitizer should be either yes or no)
endif

