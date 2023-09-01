#include "encoder/string.h"

char *PROGRAM_NAME = NULL;

void usage(int status) {
    fprintf(stderr, "Usage: %s flag\n\n", PROGRAM_NAME);
    fprintf(stderr, "   --encode INPUT_FILE     Encode strings from INPUT_FILE\n");
    exit(status);
}

int encode(char *input_file) {

    FILE *fptr = fopen(input_file,"r");
    if (!fptr) {
        printf("ERROR: File path specified does not exist.");
        exit(EXIT_FAILURE);
    }

    char *s = (char *) calloc(256, sizeof(char));
    char *es = (char *) calloc(256, sizeof(char));

    fgets(es, BUFSIZ, fptr);

    if (strlen(es) > 257) {
        printf("ERROR: The string length is %li, which larger than 255 characters.\n", strlen(es) - 2);
        exit(EXIT_FAILURE);
    }

    if (string_decode(es, s) == 0) {
        char *output_es = (char *) calloc(256, sizeof(char));
        string_encode(s, output_es);
        printf("%s\n", output_es);
        free(output_es);
    } else {
        return EXIT_FAILURE;
    }

    free(s);
    free(es);
    fclose(fptr);

    return 0;
}

int main(int argc, char *argv[]) {
    PROGRAM_NAME = argv[0];
    int argind = 1;

    while (argind < argc && strlen(argv[argind]) > 1 && argv[argind][0] == '-') {
        if (!strcmp(argv[argind], "--encode")) {
            if (!argv[argind + 1]) {
                usage(0);
            }
            return encode(argv[argind + 1]);
        } else if (!strcmp(argv[argind], "--help")) {
            usage(0);
        }
        argind++;
    }

    return 0;
}