/* standerd c library */

#include <stdio.h>
#include <string.h>

/* code base */

#include "ASM/exec.h"
#include "Linker/linkerscript.h"

/* SDK */

#include <pico.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <hardware/flash.h>
#include <hardware/sync.h>
#include <hardware/regs/addressmap.h>
#include <hardware/regs/m0plus.h>

/* deps */

#include "blockdevice/sd.h"

#include "filesystem/fat.h"
#include "filesystem/vfs.h"

#define SD_SO 12
#define SD_SI 11
#define SD_CLK 10
#define SD_CS 13

#define BOOTLOADER_OFFSET 256 * 1024

/* Global File Pointer */
  
FILE *fp;

/* Global Bin File Values */

uint8_t buffer[FLASH_SECTOR_SIZE] = {0};

size_t bytesread = 0;
size_t proinc = 0;

void app_execute() {

  /* Vector Table Reset  */

  uint32_t *new_vector_table = (uint32_t *)(XIP_BASE + BOOTLOADER_OFFSET); // new vector table destination
  volatile uint32_t *vtor = (uint32_t *)(PPB_BASE + M0PLUS_VTOR_OFFSET); // vector table location
  *vtor = (uint32_t)new_vector_table; // new vector table assignment

  //stdio_deinit_all(); // deinit all pins used as this can fuck up the other program when they also try to do so (for some reason this screws with the other program)
  //maybe because when the vector table resets everything reset hmm, lets just not fuck with it although it would be nice to have a reason

  exec(new_vector_table[0], new_vector_table[1]); // loads program 
}

int cache_check() {
 
  while ((bytesread = fread(buffer, 1, sizeof(buffer), fp)) > 0  ) {
    uint8_t *flash = (uint8_t *)(XIP_BASE + FLASH_SECTOR_SIZE + proinc);
    if (memcmp(buffer, flash, bytesread) != 0) {
      return -1;
    }
  }

  return 0;
}

void load_app(char* appname) {

  /* Filesystem Init */

  blockdevice_t *sd = blockdevice_sd_create(spi1, SD_SI, SD_SO, SD_CLK, SD_CS, 125000000 / 2 / 4, true);

  printf("%s\n", "SD card init");

  filesystem_t *fatfs = filesystem_fat_create();
  if (fatfs == NULL) {
    printf("%s\n", "failed to create fat binsystem");
  }

  if (fs_mount("/sd", fatfs, sd) == -1) {
    printf("%s\n", "sd card failed to mount");
  }

  fp = fopen(appname, "r");

  if (cache_check() == 0) {
    printf("%s\n", "Programs are the same executing");
    app_execute(); 
  }

  if(fseek(fp, 0, SEEK_SET) == -1) {
    printf("%s%s\n", "fseek failed: ", strerror(errno));
  }

  /* Bin Values Sanity Check */

  for(uint32_t i = 0; i >= sizeof(buffer); i++) { // clearing data from buffer
    buffer[i] = 0;
  }

  bytesread = 0;
  proinc = 0;  
  
  /* Load Binary */
 
  while((bytesread = fread(buffer, 1, sizeof(buffer), fp)) > 0) { 

    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(BOOTLOADER_OFFSET + proinc, FLASH_SECTOR_SIZE);
    flash_range_program(BOOTLOADER_OFFSET + proinc, buffer, bytesread);
    restore_interrupts(ints);

    printf("%d\n", bytesread); // prints amount of bytes read
    proinc += bytesread; // adjust program size (should rename this as it's used to incriment flash program size)
  }

  /* Clean Up Filsystem */

  fclose(fp);
  filesystem_fat_free(fatfs);
  blockdevice_sd_free(sd);

  app_execute();
}

int main(void) {
  stdio_init_all();
  load_app("/sd/hello.bin");
}
