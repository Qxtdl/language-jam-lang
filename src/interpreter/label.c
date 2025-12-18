#include <string.h>
#include <stddef.h>

#include "../global.h"

extern void set_pc_line(int line);

typedef struct {
   const char *name;
   int line;
} label_t;

static label_t *label_stack = NULL;
static size_t label_stack_size = 0;

void push_label(const char *name, int line) {
   label_stack = realloc(label_stack, ++label_stack_size * sizeof(label_t));
   label_stack[label_stack_size - 1].name = name;
   label_stack[label_stack_size - 1].line = line;
}

void goto_label(const char *label) {
   for (size_t i = 0; i < label_stack_size; i++)
      if (!strcmp(label_stack[i].name, label)) {
         set_pc_line(label_stack[i].line + 1);
         return;
      }
   app_abort("goto_label()", "Could not jump to a label named %s doesn't exist", label);
}