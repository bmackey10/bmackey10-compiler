#include "../parser/token.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int string_decode(const char *es, char *s, int t);
int string_encode(const char *s, char *es);
int encode_file(char *input_file);
