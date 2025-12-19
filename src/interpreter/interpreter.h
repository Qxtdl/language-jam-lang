#pragma once

#define interpreter_abort(func_name, reasson, ...) \
   { \
      printf("Interpreter aborting at line %zu\n", interpreter.pc_index); \
      app_abort(func_name, reasson, __VA_ARGS__); \
   }

extern struct interpreter {
   const char **pc;
   size_t pc_index;
} interpreter;

int interpret(char *source);