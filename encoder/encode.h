#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "../parser/token.h"

int string_decode(const char *es, char *s, int t);
int string_encode(const char *s, char *es);
int encode_file(char *input_file);
