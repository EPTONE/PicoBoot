/* standerd c library */

#include <stdio.h>
#include <string.h>
#include <errno.h>

/* code base */

#include "ASM/exec.h" // keep this at the top or else compiler will complain
#include "../Err/err.h"
#include "../Filesystem/filesystem.h"
#include "loader.h"

#include "ff.h"

/* pico-sdk */

#include <pico.h>
#include <pico/stdio.h>
#include <pico/stdlib.h>

#include <hardware/flash.h>
#include <hardware/sync.h>
#include <hardware/regs/addressmap.h>
#include <hardware/regs/m0plus.h>

#define BOOTLOADER_OFFSET 256 * 1024 // for some reason the linker script dosn't actually work on this maybe because I'm using the wrong byte format
                                     // none the less defines like these seem to be the neccesery norm for these things.

FIL binfp;
FRESULT bfr;

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

  exec_app(new_vector_table[0], new_vector_table[1]); // loads program 
}

int cache_check() {

  printf("\n\n\t=CACHE CHECK=\n\n");
 
  while (true) {

    bfr = f_read(&binfp, buffer, sizeof(buffer), &bytesread);
    if(bfr != FR_OK) {
      printf("failed to read file: %d | CACHE CHECK\n", bfr);
      err(bfr);
    }
    if(bytesread <= 0) break;

    uint8_t *flash = (uint8_t *)(XIP_BASE + BOOTLOADER_OFFSET + proinc);
    if (memcmp(buffer, flash, bytesread) != 0) {
      return 1;
    }

    printf("bytes checked: %d\n", bytesread);
    printf("total bytes checked: %d\n", proinc);

    proinc += bytesread;
  }

  return 0;
}

void load_app(const char *app_name) {

  printf("opening file: %s\n", app_name);

  bfr = f_open(&binfp, app_name, FA_READ);
  if(bfr != FR_OK) {
    printf("failed to open file: %s | BINARY READ | ERROR CODE : %d\n", app_name, bfr);
    err(bfr);
  }

 if (cache_check() == 0) {

   printf("%s\n", "Programs are the same executing");

   app_execute(); 
 }

  f_lseek(&binfp, 0);
  if (bfr != FR_OK){
    printf("f_lseek failed: BINARY READ\n");
    err(bfr);
  }

  /* Bin Values Sanity Check */

  for(uint32_t i = 0; i >= sizeof(buffer); i++) { // clearing data from buffer
    buffer[i] = 0;
  }

  bytesread = 0;
  proinc = 0;  
  
  /* Load Binary */
 
  printf("\n\n\t=BINARY LOAD=\n\n");

  while(true) {

    bfr = f_read(&binfp, buffer, sizeof(buffer), &bytesread);
    if(bfr != FR_OK) {
      printf("failed to read file: BINARY LOAD");
      err(bfr);
    }
    if(bytesread <= 0) break;

    uint32_t ints = save_and_disable_interrupts();
    flash_range_erase(BOOTLOADER_OFFSET + proinc, FLASH_SECTOR_SIZE);
    flash_range_program(BOOTLOADER_OFFSET + proinc, buffer, bytesread);
    restore_interrupts(ints);

    printf("bytesread: %d\n", bytesread); // prints amount of bytes read
    printf("total bytes read: %d\n", proinc);
    proinc += bytesread; // adjust program size (should rename this as it's used to incriment flash program size)
  }

  /* Clean Up Filsystem */

  filesystem_deinit(&binfp);

  app_execute();
}
