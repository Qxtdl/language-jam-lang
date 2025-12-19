#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../global.h"
#include "interpreter.h"
#include "variable_stack.h"

static variable_t *variable_stack = NULL;
static size_t variable_stack_size = 0;
static variable_t *variable_stack_end;

void push_variable(const char *identifier, char *value, var_type_t type) {
   if (type == VARIABLE_TYPE_STRING) {
      *strstr(++value, "\"") = '\0';
   }

   variable_stack = realloc(variable_stack, ++variable_stack_size * sizeof(variable_t));
   variable_stack_end = &variable_stack[variable_stack_size - 1];
   variable_stack[variable_stack_size - 1].identifier = strdup(identifier);
   variable_stack[variable_stack_size - 1].type = type;
   if (type == VARIABLE_TYPE_NUMBER) {
      // TODO add hex support here
      variable_stack[variable_stack_size - 1].value.int_value = strtol(value, NULL, 10);
   }
   if (type == VARIABLE_TYPE_STRING) {
      variable_stack[variable_stack_size - 1].value.string_value = strdup(value);
   }
}

variable_t pop_variable(void) {
   variable_t popped = variable_stack[variable_stack_size - 1];
   free(variable_stack[variable_stack_size - 1].identifier);
   variable_stack = realloc(variable_stack, --variable_stack_size * sizeof(variable_t));
   return popped;
}

variable_t *lookup_variable(const char *identifier) {
   for (size_t i = 0; i < variable_stack_size; i++)
      if (!strcmp(variable_stack[i].identifier, identifier))
         return &variable_stack[i];
   interpreter_abort("lookup_variable()", "Could not look up variable %s", identifier);
}

inline void save_variable(variable_t *variable, var_type_t new_type, union variable_value variable_value) {
   if (variable->type == VARIABLE_TYPE_STRING)
      free(variable->value.string_value);
   variable->type = new_type;
   variable->value = variable_value;
}