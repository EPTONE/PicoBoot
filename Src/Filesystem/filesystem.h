#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include "ff.h"

/* Filesystem Pointer */

extern FATFS fatfs;

/* Functions */

int filesystem_init();
void filesystem_deinit(FIL *file_fp);

#endif
