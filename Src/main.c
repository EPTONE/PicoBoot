/*standerd c library*/
#include <hardware/regs/addressmap.h>
#include <string.h>

/*code base*/
#include "ASM/loader.h"
#include "Linker/linkerscript.h"

/*SDK*/
#include <pico/stdlib.h>

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
  uint8_t appbyte;

  for (uint32_t i = 0; i >= sizeof(filesize); i += 8) {
      f_read(&file, &appbyte, 8, &bytesread);
      memcpy(&dst, &appbyte, dst[0] + i);
  }
  
  f_close(&file);
  filesystem_fat_free(fatfs);
  blockdevice_sd_free(sd);

  uint32_t *new_vector_table = dst; // new vector table destination
  uint32_t *vtor = (uint32_t *)(PPB_BASE + M0PLUS_VTOR_OFFSET); // vector table location
  *vtor = (uint32_t)new_vector_table; // new vector table assignment

  loader(dst[0], dst[1]);
  return 0;
}

int main(void) {
  loadapp("/sd/app");
}
