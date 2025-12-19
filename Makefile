CC = gcc
CFLAGS = -Wall -Wextra -Werror -DSTRAIGHT_ECHO \
	-Wno-error=format-security -Wno-error=unused-parameter

BUILD = build
OUTPUT = $(BUILD)/ljlang
OUTPUT_ARGS = examples/test.ljl

CSRCS = $(shell find src -name '*.c')
OBJS = $(patsubst %.c,$(BUILD)/%.o,$(CSRCS))

all: clean $(OBJS) $(OUTPUT) run

$(BUILD)/%.o : %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUTPUT): $(OBJS)
	gcc -o $(OUTPUT) $(OBJS)

run:
	./$(OUTPUT) $(OUTPUT_ARGS)

clean:
	rm -rf $(BUILD)