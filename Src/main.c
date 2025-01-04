/*standerd c library*/
#include <hardware/regs/addressmap.h>
#include <pico/stdio.h>
#include <stdio.h>

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

#define SD_SO 12
#define SD_SI 11
#define SD_CLK 10
#define SD_CS 13

int loadapp(char* appname) {

  blockdevice_t *sd = blockdevice_sd_create(spi1, SD_SI, SD_SO, SD_CLK, SD_CS, 125000000 / 2 / 4, true);

  printf("%s\n", "SD card init");

  filesystem_t *fatfs = filesystem_fat_create();
  if (fatfs == NULL) {
    printf("%s\n", "failed to creat fat binsystem");
  }

  int err = fs_mount("/sd", fatfs, sd);
  if (err == -1) {
    printf("%s\n", "sd card failed to mount");
    return -1;
  }

  printf("%s\n", "binsystem mounted");

  FIL bin;

  f_open(&bin, appname, FA_READ);

  printf("%s\n", "bin opened");

  uint32_t *dst = (uint32_t *)__APP_START__;

  size_t bytesread = 0;
  uint32_t binsize = f_size(&bin);
  //uint32_t *appbyte = 0;
  
  printf("%s\n", "reading binary file");
 
  for(uint32_t i = 0; i <= binsize; i++) {
    f_read(&bin, &__APP_START__, 1, &bytesread); 
    printf("%d\n", bytesread);
  }

  //printf("%ld", *appbyte);
  //memcpy((uint32_t *)__APP_START__, &appbyte, sizeof(*appbyte)); //try this first if no work we'll just move the dst++ down to the last line

  f_close(&bin);
  filesystem_fat_free(fatfs);
  blockdevice_sd_free(sd);

  printf("%s\n", "bin clean up finished");

//  uint32_t *new_vector_table = (uint32_t *)dst[0]; // new vector table destination
//  uint32_t *vtor = (uint32_t *)__BOOTLOADER_START__; // vector table location
//  *vtor = (uint32_t)new_vector_table; // new vector table assignment

// printf("%s\n", "vector table updated");
  printf("%s\n", "loading program");

  stdio_deinit_all();
  
  loader(dst[0], dst[1]);
  return 0;
}

int main(void) {
  stdio_init_all();
  loadapp("pico.bin");
}
