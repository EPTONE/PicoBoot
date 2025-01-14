#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "blockdevice/blockdevice.h"
#include "filesystem/filesystem.h"
#include <stdio.h>
#include <blockdevice/sd.h>
#include <filesystem/fat.h>

#define SD_SO 12
#define SD_SI 11
#define SD_CLK 10
#define SD_CS 13

/* File Pointer  */

extern FILE *fp;

/* SD Card Pointer */

extern blockdevice_t *sd;

/* Filesystem Pointer */

extern filesystem_t *fatfs;

/* Functions */

int filesystem_init();
void filesystem_deinit();

#endif
