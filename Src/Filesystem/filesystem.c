/* C Libary Headers  */
#include <stdio.h>
#include <errno.h>

/* Deps  */
#include <filesystem/vfs.h>
#include <filesystem/fat.h>
#include <blockdevice/sd.h>

/* Code Base */
#include "../Err/err.h"
#include "filesystem.h"

/* File Pointer */
  
FILE *fp;

/* SD Card Pointer */

blockdevice_t *sd;

/* Filesystem Pointer */

filesystem_t *fatfs;

int filesystem_init() {

  sd = blockdevice_sd_create(spi1, SD_SI, SD_SO, SD_CLK, SD_CS, 125000000 / 2 / 4, true);

  printf("%s\n", "SD card init");

  fatfs = filesystem_fat_create();
  if (fatfs == NULL) {
    printf("%s\n", "failed to create fat filesystem");
    err(errno);
  }

  if (fs_mount("/sd", fatfs, sd) == -1) {
    printf("%s\n", "sd card failed to mount");
    err(errno);
  }

  return 0;
}

void filesystem_deinit() {
  fclose(fp);
  filesystem_fat_free(fatfs);
  blockdevice_sd_free(sd);
}
