#include "string.h"


int string_decode(const char *es, char *s) {

    int s_count = 0;

    if (es[0] != '\"') {
        printf("ERROR: The string is not properly enclosed in double quotes.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < strlen(es) - 1; i++) {
        
        if (es[i] == '\"') {
            printf("ERROR: The string contains an unescaped double quote.\n");
            exit(EXIT_FAILURE);
        } else if (es[i] == '\\') {
            i++;
            if (i == strlen(es) - 1) {
                printf("ERROR: The string is not properly enclosed in double quotes.\n");
                exit(EXIT_FAILURE);
            }
            switch (es[i]) {
                case 'a':
                    s[s_count] = '\a';
                    break;
                case 'b':
                    s[s_count] = '\b';
                    break;
                case 'e':
                    s[s_count] = '\e';
                    break;
                case 'f':
                    s[s_count] = '\f';
                    break;
                case 'n':
                    s[s_count] = '\n';
                    break;
                case 'r':
                    s[s_count] = '\r';
                    break;
                case 't':
                    s[s_count] = '\t';
                    break;
                case 'v':
                    s[s_count] = '\v';
                    break;
                case '\\':
                    s[s_count] = '\\';
                    break;
                case '\'':
                    s[s_count] = '\'';
                    break;
                case '\"':
                    s[s_count] = '\"';
                    break;
                case '0':
                    if (i == strlen(es) - 4) {
                        printf("ERROR: The string is not properly enclosed in double quotes.\n");
                        exit(EXIT_FAILURE);
                    }
                    if (es[i + 1] == 'x' && isalnum(es[i + 2]) && isalnum(es[i + 3])) {
                        if (isalpha(es[i + 2]) && ((es[i + 2] >= 71 && es[i + 2] <= 90) || (es[i + 2] >= 103 && es[i + 2] <= 122))) {
                            printf("ERROR: The hexidecimal %c%c code does not exist.\n", es[i + 2], es[i + 3]);
                            exit(EXIT_FAILURE);
                        }
                        if (isalpha(es[i + 3]) && ((es[i + 3] >= 71 && es[i + 3] <= 90) || (es[i + 3] >= 103 && es[i + 3] <= 122))) {
                            printf("ERROR: The hexidecimal %c%c code does not exist.\n", es[i + 2], es[i + 3]);
                            exit(EXIT_FAILURE);
                        }
                        char temp[2] = {es[i + 2], es[i + 3]};
                        if (strtol(temp, 0, 16) > 255) {
                            printf("ERROR: The hexidecimal code is out of range.\n");
                            exit(EXIT_FAILURE);
                        } else if (strtol(temp, 0, 16) == 0) {
                            printf("ERROR: The hexidecimal %c%c code does not exist.\n", es[i + 2], es[i + 3]);
                            exit(EXIT_FAILURE);
                        } else {
                            s[s_count] = (int) strtol(temp, 0, 16);
                        }
                        i = i + 3;
                    } else {
                        printf("ERROR: The hexidecimal code does not exist.\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
                default:
                    printf("ERROR: The backslash code \\%c does not exist.\n", es[i]);
                    exit(EXIT_FAILURE);
            }
        } else if (((int) es[i]) <= 126 && ((int) es[i]) >= 32) {
            s[s_count] = es[i];
        } else {
            printf("ERROR: The value %c's ASCII value is out of range or it contains in unknown character.\n", es[i]);
            exit(EXIT_FAILURE);
        }

        s_count++;
    }

    s[s_count - 1] = '\0';

    return 0;
}

int string_encode(const char *s, char *es) {

    int es_count = 1;

    *es = '\"';

    for (int i = 0; i < strlen(s); i++) {
        
        switch (s[i]) {
            case '\a':
                es[es_count] = '\\';
                es[es_count + 1] = 'a';
                es_count++;
                break;
            case '\b':
                es[es_count] = '\\';
                es[es_count + 1] = 'b';
                es_count++;
                break;
            case '\e':
                es[es_count] = '\\';
                es[es_count + 1] = 'e';
                es_count++;
                break;
            case '\f':
                es[es_count] = '\\';
                es[es_count + 1] = 'f';
                es_count++;
                break;
            case '\n':
                es[es_count] = '\\';
                es[es_count + 1] = 'n';
                es_count++;
                break;
            case '\r':
                es[es_count] = '\\';
                es[es_count + 1] = 'r';
                es_count++;
                break;
            case '\t':
                es[es_count] = '\\';
                es[es_count + 1] = 't';
                es_count++;
                break;
            case '\v':
                es[es_count] = '\\';
                es[es_count + 1] = 'v';
                es_count++;
                break;
            default:
                if (((int) s[i]) <= 126 && ((int) s[i]) >= 32) {
                    es[es_count] = s[i];
                } else if ((((int) s[i]) > 126 && ((int) s[i]) <= 255) || ((int) s[i]) < 32) {
                    char hex_value[2];
                    unsigned char hex_temp = s[i];
                    sprintf(hex_value, "%x", hex_temp);
                    es[es_count] = '\\';
                    es[es_count + 1] = '0';
                    es[es_count + 2] = 'x';
                    es[es_count + 3] = hex_value[0];
                    es[es_count + 4] = hex_value[1];
                    es_count = es_count + 4;
                } else {
                    printf("ERROR: The value %c's ASCII value is out of range or it contains in unknown character.\n", es[i]);
                    exit(EXIT_FAILURE);
                }
        }

        es_count++;
    }

    es[es_count] = '\"';

    return 0;
}
