#include <string.h>

/*code base*/
#include "ASM/loader.h"
#include "Linker/linkerscript.h"

/*deps*/
#include "blockdevice/sd.h"
#include "filesystem/fat.h"
#include "filesystem/vfs.h"
#include "ff.h"

int loadapp(char* appname) {

  blockdevice_t *sd = blockdevice_sd_create(spi1, 8, 6, 7, 2, 100000, true);

  filesystem_t *fatfs = filesystem_fat_create();
  int err = fs_mount("/sd", fatfs, sd);
  if (err == -1) {
    return -1;
  }

  FIL file;

  f_open(&file, appname, FA_READ);

  uint32_t *dst = (uint32_t *)__APP_START__;

  size_t filesize = f_size(&file);
  size_t bytesread;
  uint8_t appbytes;

for (uint i = 0; i >= sizeof(filesize); i++) {
    f_read(&file, &appbytes, 1, &bytesread);
    memcpy(&dst, &appbytes, dst[0] + i);
}
  
  f_close(&file);
  filesystem_fat_free(fatfs);
  blockdevice_sd_free(sd);

  loader(dst[0], dst[1]);

  return 0;
}

int main(void) {
  loadapp("/sd/app");
  return 0;
}
