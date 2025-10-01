# ========= project config =========
SRC_DIR 		:= src
BUILD   		:= build
LIBDIR 			:= lib
# find out all .c file(including sub dir)
SRC := $(shell find $(SRC_DIR) -name '*.c')
# put all .o file in build dir(with out sub dir)
OBJ := $(addprefix $(BUILD)/, $(notdir $(SRC:.c=.o)))
DEP := $(OBJ:.o=.d)

TEST_SRC_DIR    := tests
TEST_SRC		:= $(wildcard $(TEST_SRC_DIR)/*.c)
TEST_BUILD		:= tests_build
TEST_BIN_DIR	:= tests_bin
TEST_OBJ		:= $(patsubst tests/%.c,$(TEST_BUILD)/%.o,$(TEST_SRC))
TEST_BIN 		:= $(patsubst tests/%.c,$(TEST_BIN_DIR)/%.exe,$(TEST_SRC))
TEST_DEP		:= $(TEST_OBJ:.o=.d)

AR      := ar
CC      ?= gcc
CFLAGS  ?= -Wall -Wextra -fPIC -MMD -MP -I$(SRC_DIR)/include -I$(TEST_SRC_DIR)/include
LDFLAGS := -shared




# out put lib file
STATIC_LIB := $(LIBDIR)/libdataStruct.a
SHARED_LIB := $(LIBDIR)/ibdataStructt.so

# ========= rules =========
# static lib
$(STATIC_LIB): $(OBJ)
	@mkdir -p $(LIBDIR)
	$(AR) rcs $@ $^

# dymanic lib
$(SHARED_LIB): $(OBJ)
	@mkdir -p $(LIBDIR)
	$(CC) $(LDFLAGS) -o $@ $^



# common rule：build/foo.o <- src/**/foo.c
# using vpath search source file in [SRC_DIR] 
vpath %.c $(shell find $(SRC_DIR) -type d)

$(BUILD)/%.o: %.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_BUILD)/%.o: $(TEST_SRC_DIR)/%.c
	@mkdir -p $(TEST_BUILD)
	$(CC) $(CFLAGS) -c $< -o $@


$(TEST_BIN_DIR)/%.exe: $(TEST_BUILD)/%.o $(OBJ)
	@mkdir -p $(TEST_BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	./$@

# if change .h file, this will make re compile
-include $(DEP) $(TEST_DEP)

# ========= tool command =========
.PHONY: clean test all

all: $(STATIC_LIB) $(SHARED_LIB)

clean:
	rm -rf $(BUILD) $(LIBDIR) $(TEST_BUILD) $(TEST_BIN_DIR)

test: CFLAGS += -g -DENABLE_DEBUG
test: $(TEST_BIN)
