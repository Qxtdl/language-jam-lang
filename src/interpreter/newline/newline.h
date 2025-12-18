#pragma once

#include <stddef.h>

typedef struct {
   struct {
      const char **lines;
      size_t amount;
      size_t size;
   } line_entries;
} lexed_source_code_t;

lexed_source_code_t lex(char *source);