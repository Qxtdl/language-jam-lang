#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "newline.h"

lexed_source_code_t lex(char *source) {
   lexed_source_code_t lexed_source_code = {0};

   char *token = strtok(source, "\n");
   while (token != NULL) {
      if (!strncmp(token, "//", 2)) {
         token = strtok(NULL, "\n");
         continue;
      }
      lexed_source_code.line_entries.lines = realloc(
         lexed_source_code.line_entries.lines, 
         ++lexed_source_code.line_entries.size * sizeof(char *)
      );
      
      lexed_source_code.line_entries.lines[lexed_source_code.line_entries.amount++] = token;
      token = strtok(NULL, "\n");
   }

   return lexed_source_code;
}