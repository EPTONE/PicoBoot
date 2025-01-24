#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "blockdevice/blockdevice.h"
#include "filesystem/filesystem.h"
#include <stdio.h>
#include <blockdevice/sd.h>
#include <filesystem/fat.h>

/* SD Card Pointer */

extern blockdevice_t *sd;

/* Filesystem Pointer */

extern filesystem_t *fatfs;

/* Functions */

int filesystem_init();
void filesystem_deinit(FILE *file_fp);

#endif
