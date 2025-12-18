#pragma once

typedef enum {
   VARIABLE_TYPE_NUMBER,
   VARIABLE_TYPE_STRING
} var_type_t;

typedef struct {
   char *identifier;
   var_type_t type;
   union variable_value {
      int int_value;
      char *string_value;
   } value;
} variable_t;

void push_variable(const char *identifier, char *value, var_type_t type);
variable_t pop_variable(void);
variable_t *lookup_variable(const char *identifier);
void save_variable(variable_t *variable, var_type_t new_type, union variable_value variable_value);
