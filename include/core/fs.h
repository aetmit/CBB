#ifndef CBB_CORE_FS_H
#define CBB_CORE_FS_H

// returns an owned buffer
// must be freed by caller
char *read_file(const char *path);

#endif // CBB_CORE_FS_H
