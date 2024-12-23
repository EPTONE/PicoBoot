/*code base*/
#include "ASM/loader.h"

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

  size_t filesize = f_size(&file);
  size_t bytesread;
  uint8_t app[filesize];
  
  f_read(&file, app, filesize, &bytesread);

  f_close(&file);
  filesystem_fat_free(fatfs);
  blockdevice_sd_free(sd);

  loader(app[1], app[0]);

  return 0;
}

int main(void) {
  loadapp("app");
  return 0;
}
