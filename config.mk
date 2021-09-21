CXX ?= clang++
CXXWARN ?= -Werror -Wall -Wextra -Wno-unused-parameter -pedantic

CXXSTD ?= c++17
SRC=src/main.cpp

CXXFLAGS+=-fno-exceptions -fno-rtti

BUILD_DIR=build
TARGET=pvc

# Unicode
UNICODE_HPP=src/lib/unicode_internal.hpp
UNICODE_GEN=$(wildcard unicode/*)
UNICODE_DATA=unicode/UnicodeData.txt
UNICODE_PY=unicode/unicode.py

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
	CXXFLAGS+=-Og -g -march=native -fno-omit-frame-pointer
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

