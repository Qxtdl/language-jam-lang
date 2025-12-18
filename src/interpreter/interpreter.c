#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "newline/newline.h"
#include "variable_stack.h"
#include "label.h"

#define CHECK_CMD(x, y) if (!strncmp(x, y, strlen(y)))
#define ELSE_CHECK_CMD(x, y) else if (!strncmp(x, y, strlen(y)))

lexed_source_code_t lexed_source_code;

struct {
   const char **pc;
   size_t pc_index;
} interpreter;

static const char **increment_pc(void) {
   const char **interpreter_old_pc = interpreter.pc;
   interpreter.pc++;
   interpreter.pc_index++;
   if (interpreter.pc_index > lexed_source_code.line_entries.amount) {
      printf("End of file (EOF) reached. Exiting\n");
      exit(0);
   }
   return interpreter_old_pc;
}

void set_pc_line(int line) {
   interpreter.pc = lexed_source_code.line_entries.lines + line - 1;
   interpreter.pc_index = line - 1;
}

const char *interpret_line(const char *_line) {
   char *line = strdup(_line);

   CHECK_CMD(line, "push") {
      strtok(line, " ");
      char *identifier = strtok(NULL, " ");
      char *value = strtok(NULL, " ");

      if (isdigit(*value))
         push_variable(identifier, value, VARIABLE_TYPE_NUMBER);
      else if (*value == '\"')
         push_variable(identifier, value, VARIABLE_TYPE_STRING);
   }
   ELSE_CHECK_CMD(line, "pop") {
      pop_variable();
   }
   ELSE_CHECK_CMD(line, "math") {
      strtok(line, " ");
      char *dest = strtok(NULL, " ");
      char *operator = strtok(NULL, " ");
      char *operand = strtok(NULL, " ");

      switch (*operator) {
         case '+':
         save_variable(lookup_variable(dest), VARIABLE_TYPE_NUMBER, (union variable_value){
            lookup_variable(dest)->value.int_value + lookup_variable(operand)->value.int_value
         });
         break;
         case '-':
         save_variable(lookup_variable(dest), VARIABLE_TYPE_NUMBER, (union variable_value){
            lookup_variable(dest)->value.int_value - lookup_variable(operand)->value.int_value
         });
         break;
         case '*':
         save_variable(lookup_variable(dest), VARIABLE_TYPE_NUMBER, (union variable_value){
            lookup_variable(dest)->value.int_value * lookup_variable(operand)->value.int_value
         });
         break;
         case '/':
         save_variable(lookup_variable(dest), VARIABLE_TYPE_NUMBER, (union variable_value){
            lookup_variable(dest)->value.int_value / lookup_variable(operand)->value.int_value
         });
         break;   
         default: printf("Unknown operator \"%s\"\n", operator); break;
      }
   }
   ELSE_CHECK_CMD(line, "move") {
      strtok(line, " ");
      char *dest = strtok(NULL, " ");
      char *operand = strtok(NULL, " ");

      save_variable(lookup_variable(dest), lookup_variable(operand)->type, lookup_variable(operand)->value);
   }
   ELSE_CHECK_CMD(line, "setpc") {
      strtok(line, " ");
      int line = strtoul(strtok(NULL, " "), NULL, 10);

      set_pc_line(line);
   }
   ELSE_CHECK_CMD(line, "goto") {
      strtok(line, " ");
      char *label = strtok(NULL, " ");
      char *operand = strtok(NULL, " ");
      char *operand2 = strtok(NULL, " ");

      if (!operand)
         goto_label(label);
      else {
         variable_t *first_to_compare = lookup_variable(operand);
         variable_t *second_to_compare = lookup_variable(operand2);

         if (first_to_compare->type == VARIABLE_TYPE_NUMBER) {
            if (first_to_compare->value.int_value == second_to_compare->value.int_value)
               goto_label(label);
         }
         else if (first_to_compare->type == VARIABLE_TYPE_STRING)
            if (!strcmp(first_to_compare->value.string_value, second_to_compare->value.string_value))
               goto_label(label);
      }
   }
   ELSE_CHECK_CMD(line, "echo") {
      strtok(line, " ");
      char *identifier = strtok(NULL, " ");
      
      variable_t *lookup = lookup_variable(identifier);
      if (lookup->type == VARIABLE_TYPE_NUMBER)
         printf("echo: %d\n", lookup->value.int_value);
      else if (lookup->type == VARIABLE_TYPE_STRING)
         printf("echo: %s\n", lookup->value.string_value);
   }
   ELSE_CHECK_CMD(line, "wait") {
      strtok(line, " ");
      sleep(strtoul(strtok(NULL, " "), NULL, 10));
   }

   free(line);
   return _line;
}

int interpret(char *source) {
   lexed_source_code = lex(source);
   interpreter.pc = lexed_source_code.line_entries.lines;
   interpreter.pc_index = 0;

   /* we need to check for labels first */
   for (size_t i = 0; i < lexed_source_code.line_entries.amount; i++) {
      if (strstr(lexed_source_code.line_entries.lines[i], ":"))
         push_label(strtok((char *)lexed_source_code.line_entries.lines[i], ":"), i + 1);
   }

   while (1)
      interpret_line(*increment_pc());

   return 0;
}