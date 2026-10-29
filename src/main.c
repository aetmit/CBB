#include <stdlib.h>
#include <stdio.h>

#include "core/fs.h"

int main(int argc, const char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "no file specified\n");
        return 1;
    }

    char *file = read_file(argv[1]);

    free(file);

    return 0;
}