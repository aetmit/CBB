#include "core/fs.h"

#include <stdlib.h>
#include <stdio.h>

char *read_file(const char *path) {
    if (!path) {
        return nullptr;
    }

    FILE *f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    rewind(f);

    char *buffer = malloc(len + 1);
    fread(buffer, 1, len, f);

    buffer[len] = '\0';

    fclose(f);

    return buffer;
}