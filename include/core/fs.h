#ifndef CBB_CORE_FS_H
#define CBB_CORE_FS_H

// returns an owned buffer
// must be freed by caller
// const because it is not logical to modify a file buffer even though there is no restriction on this
const char *read_file(const char *path);

#endif // CBB_CORE_FS_H
