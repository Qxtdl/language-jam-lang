#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter/interpreter.h"

static void usage(void) {
   printf("Usage: ljlang <.ljl file to interpret>\n");
   exit(0);
}

int main(int argc, char **argv) {
   if (argc <= 1 || (argc >= 1 && !strcmp(argv[1], "--help"))) {
      usage();
   }

   FILE *fptr;
   fptr = fopen(argv[1], "r");
   fseek(fptr, 0, SEEK_END);
   long fsize = ftell(fptr);
   rewind(fptr);
   char *file = malloc(fsize + 1);
   fread(file, 1, fsize, fptr);
   file[fsize] = '\0';
   fclose(fptr);

   interpret(file);
}