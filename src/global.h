#pragma once

#include <stdio.h>
#include <stdlib.h>

#define app_abort(func_name, reasson, ...) \
   { \
      fprintf(stderr, "[ABORT]: %s:" func_name ":%d: " reasson "\n", __FILE_NAME__, __LINE__, ##__VA_ARGS__); \
      exit(1); \
   }

#define quick_abort(reasson, ...) \
   { \
      fprintf(stderr, "[ABORT]: %s:%d: " reasson "\n", __FILE_NAME__, __LINE__, ##__VA_ARGS__); \
      exit(1); \
   }

#define NOT_IMPLEMENTED quick_abort("NOT IMPLEMENTED!")