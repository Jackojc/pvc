# pvc

.POSIX:

include config.mk

all: options pvc

config:
	@mkdir -p $(BUILD_DIR)

options:
	@printf "cc \033[32m$(CXX)\033[0m | "
	@printf "dbg \033[32m$(debug)\033[0m | "
	@printf "san \033[32m$(sanitizer)\033[0m | "
	@printf "cflags \033[32m-std=$(CXXSTD) $(CXXFLAGS)\033[0m\n"

pvc: config
	@$(CXX) -std=$(CXXSTD) $(CXXWARN) $(CXXFLAGS) $(LDFLAGS) $(CPPFLAGS) $(INC) \
		$(LIBS) -o $(BUILD_DIR)/$(TARGET) $(SRC)

clean:
	rm -rf $(BUILD_DIR)/ *.gcda

$(TEST_TARGET): config
	@$(CXX) -std=$(CXXSTD) $(CXXWARN) $(CXXFLAGS) $(LDFLAGS) $(CPPFLAGS) $(INC) \
		$(LIBS) -o $@ $(TEST_DIR)/$(basename $(notdir $@)).cpp

test: $(TEST_TARGET)
	@( BUILD_DIR=$(BUILD_DIR) TEST_DIR=$(TEST_DIR) TEST_FILE=$(TEST_FILE) ./test.sh )

.PHONY: all options clean test

