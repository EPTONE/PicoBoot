/*standerd c library*/
#include <hardware/regs/addressmap.h>
#include <hardware/regs/m0plus.h>
#include <pico.h>
#include <pico/stdio.h>
#include <stdio.h>

/*code base*/
#include "ASM/loader.h"
#include "Linker/linkerscript.h"

/*SDK*/
#include <pico/stdlib.h>
#include <hardware/flash.h>
#include <hardware/sync.h>

/*deps*/
#include "blockdevice/sd.h"
#include "filesystem/fat.h"
#include "filesystem/vfs.h"

#define SD_SO 12
#define SD_SI 11
#define SD_CLK 10
#define SD_CS 13

#define BOOTLOADER_OFFSET 256 * 1024

void loadapp(char* appname) {

  blockdevice_t *sd = blockdevice_sd_create(spi1, SD_SI, SD_SO, SD_CLK, SD_CS, 125000000 / 2 / 4, true);

  printf("%s\n", "SD card init");

  filesystem_t *fatfs = filesystem_fat_create();
  if (fatfs == NULL) {
    printf("%s\n", "failed to creat fat binsystem");
  }

  int err = fs_mount("/sd", fatfs, sd);
  if (err == -1) {
    printf("%s\n", "sd card failed to mount");
  }

  printf("%s\n", "binsystem mounted");

  FILE *bin = fopen(appname, "r");

  printf("%s\n", "bin opened");

  uint32_t *dst = (uint32_t *)(XIP_BASE + BOOTLOADER_OFFSET);
  uint8_t buffer[FLASH_SECTOR_SIZE] = {0};
  size_t bytesread = 0;
  size_t program_size = 0;
  
  printf("%s\n", "reading binary file");
 
  for(uint32_t i = 0; (bytesread = fread(buffer, 1, sizeof(buffer), bin)) > 0; i++) { 

    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(BOOTLOADER_OFFSET + program_size, FLASH_SECTOR_SIZE);
    flash_range_program(BOOTLOADER_OFFSET + program_size, buffer, bytesread);
    restore_interrupts(ints);
    printf("%d\n", bytesread);
    program_size += bytesread; 
  }

  fclose(bin);
  filesystem_fat_free(fatfs);
  blockdevice_sd_free(sd);

  printf("%s\n", "bin clean up finished");

  uint32_t *new_vector_table = dst; // new vector table destination
  volatile uint32_t *vtor = (uint32_t *)(PPB_BASE + M0PLUS_VTOR_OFFSET); // vector table location
  *vtor = (uint32_t)new_vector_table; // new vector table assignment

  loader(new_vector_table[0], new_vector_table[1]);
//  return 0;
}

int main(void) {
  stdio_init_all();
  loadapp("/sd/hello.bin");
}
